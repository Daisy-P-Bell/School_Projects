// Client.c - client source file for RFT protocol

#include "state.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define IP_SRC "192.168.1.102" //source IP, might want to change
#define IP_DST "10.10.1.101"  //destination IP, ^
#define UDP_SRC 54321   //source port (might want to change)
#define UDP_DST 12345   //destination port (might want to change)

int init_socket();

//SERVER_IP: ip address of the RFT server
//SERVER_PORT: network port number where server is listening
//REMOTE_PATH: filesystem path on the server for the file to be downloaded
//LOCAL_PATH: filesystem path on the client machine where to store the downloaded file
int main(int argc, char* argv[]) {
    char* SERVER_IP, * SERVER_PORT, * REMOTE_PATH, * LOCAL_PATH;
    if (argc == 5) {
        SERVER_IP = argv[1];
        SERVER_PORT = argv[2];
        REMOTE_PATH = argv[3];
        LOCAL_PATH = argv[4];
    } else {
        printf("ERROR: Expected 4 arguments but found %d instead!\n", argc - 1);
        return 1;
    }
    // test connection

    // verify local path
    FILE* fptr = fopen(LOCAL_PATH, "w");
    if (fptr == NULL) {
        printf("ERROR: LOCAL_PATH directory is invalid.\n");
    }

    Timer timer;
    ClientState state = {
            init, 0, timer
    };

    int socket_desc = init_socket();

    struct sockaddr_in dest_addr; //socket address info, destination address
    dest_addr.sin_family = AF_INET; //the destination address info, ip version 4
    dest_addr.sin_port = htons(UDP_DST); //the desired port (destination port)
    dest_addr.sin_addr.s_addr = inet_addr(IP_DST); //destination ip address



//    Where we are storing the file's contents
    struct Data data;
    int retries = 0;

    char* datagram = make_pkt(strlen(REMOTE_PATH), 0, REMOTE_PATH, IP_SRC, IP_DST, UDP_SRC, UDP_DST, strlen(REMOTE_PATH));

    start_timer(&state.timer);
    rft_send(datagram, socket_desc, dest_addr);

    int finished = 0;
    while (!finished) {
        switch (state.status) {
            case init:
            case send_ACK: {
                // Check to see if there are bytes to receive - store them in data
                int bytes_received = rft_rcv(socket_desc, &data);
                //printf("%d bytes received\n",bytes_received);
                if (bytes_received > 0) {
                    state.status = receive_SEQ;
                } else {
                    if (!state.timer.stopped) {
                        if (state.timer.num_retries == MAX_RETRIES) {
                            printf("Error sending datagram - maximum number of retries reached\n");
                            finished = 1;
                            // First time sending
                        } else {
                            rft_send(datagram, socket_desc, dest_addr);
                            state.timer.num_retries++;
                        }
                    }
                }
                break;
            }
            case receive_SEQ: {
                if (!is_corrupt(&data)) {
                    if (has_SEQ(&data, -1)) {
                        printf("Error from server - %s",data.data);
                        finished = 1;
                    }
                    else if (has_SEQ(&data, -2)) {
                        printf("File transfer finished!\n");
                        finished = 1;
                    } else if (data.SEQ > state.expected_SEQ){
//                        Store the size of the data
                        int data_size = data.SEQ - state.expected_SEQ;
                        //printf("Writing %d bytes to the file\n", data_size);
//                        Updated the expected_SEQ (which is also what we send as an ACK)
                        state.expected_SEQ = data.SEQ;
                        // don't write first server response to file
                        if (data.SEQ != strlen(REMOTE_PATH)) {
                            fwrite(data.data, 1, data_size, fptr);
                            fwrite(data.data, 1, data_size, stdout);
                        }
                        datagram = make_pkt(state.expected_SEQ, state.expected_SEQ, "", IP_SRC, IP_DST, UDP_SRC, UDP_DST, 0);                  //uncomment
                    } else {
                        printf("data.SEQ = %d, state.expected_SEQ = %d", data.SEQ, state.expected_SEQ);
                    }
                } else {
                    printf("Corrupt data\n");
                    // resend last datagram
                }
                start_timer(&state.timer);
                state.status = send_ACK;
                break;
            }
            default: {
                // shouldn't reach
                finished = 1;
            }
        }
    }
    fclose(fptr);
}

int init_socket() {
    //open up a socket (for udp)
    struct addrinfo hints;
    struct addrinfo * address_resource;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM; // Streaming Protocol (sets resulting ai_protocol to TCP on most systems)
    hints.ai_flags = AI_PASSIVE;     // Listen

    // change this to use UDP_SRC
    getaddrinfo(IP_SRC, "54321", &hints, &address_resource);

    int status = 0;
    int socket_desc = socket(AF_INET, SOCK_DGRAM, 0); //(SOCK_RAW is what requires root), this specifies UDP
    if (socket_desc < 0) {  //error checking
        printf("Error socket(): %s\n", strerror(errno));
    }

    //set socket options
    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10000;

    status = setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));  //actually sets the socket options
    if (status < 0) {
        printf("Error setsockopt(): %s\n", strerror(errno));
    }
    int tmp_ret = bind(socket_desc, address_resource->ai_addr, address_resource->ai_addrlen);


    freeaddrinfo(address_resource);

    return socket_desc;
}


