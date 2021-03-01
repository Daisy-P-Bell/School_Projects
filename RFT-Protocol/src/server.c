// Server.c - server source file for RFT protocol

#include "state.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_SRC "10.10.1.101"  //source IP, might want to change
#define IP_SRC "10.10.1.101" //source IP, might want to change
#define IP_DST "192.168.1.102"  //destination IP, ^
#define UDP_SRC 12345  //source port (might want to change)
#define UDP_DST 54321  //destination port (might want to change)  //I think this is the one the arguements want to change, check line 61
#define BACKLOG 10

unsigned short checksum1(const char *buf, unsigned size);
char * create_datagram(char * data);
void split_file(char * FILE_PATH);

//LISTENING_PORT: network port number where server is listening
int main(int argc, char* argv[]) {

    struct ip ip_header;  //the struct is found in another library, comes with members that we fill in for the ip header standard
    struct udphdr udp_header; // the struct is found in another library, comes with members that we fill in for a udp header
    char * test = "Hello World!"; //will want to change this data storage later probably
    int LISTENING_PORT;
    char * listenn;

    //argument check
    if (argc == 2) {
        listenn = argv[1];
        LISTENING_PORT = atoi(listenn);
    } else {
        printf("ERROR: Expected 1 argument but found %d instead!", argc - 1);
        return 1;
    }

    //store the file path from initial client packet
    char* FILE_PATH = "/home/daisy/script.txt";

    struct addrinfo hints;
    struct addrinfo * address_resource;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM; // Streaming Protocol (sets resulting ai_protocol to TCP on most systems)
    hints.ai_flags = AI_PASSIVE;     // Listen

    getaddrinfo(IP_SRC, "12345", &hints, &address_resource);

    //open up a socket (for udp)
    struct sockaddr_in dest_addr; //socket address info, destination address
    dest_addr.sin_family = AF_INET; //the destination address info, ip version 4
    dest_addr.sin_port = htons(LISTENING_PORT); //the desired port (destination port)
    dest_addr.sin_addr.s_addr = inet_addr(IP_DST); //destination ip address

    //create the socket
    int status = 0;
    int socket_desc = socket(AF_INET, SOCK_DGRAM, 0); //(SOCK_RAW is what requires root), this specifies UDP
    //int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {  //error checking
        printf("Error socket(): %s\n", strerror(errno));
    }
    struct timeval read_timeout;
    read_timeout.tv_sec = 10;
    read_timeout.tv_usec = 0;

    status = setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

    int tmp_ret = bind(socket_desc, address_resource->ai_addr, address_resource->ai_addrlen);

    if (tmp_ret != 0) {
        printf("Error: %s\n", strerror(errno));
        return 1;
    }
    //listen(socket_desc, BACKLOG);

    //    bind(socket_desc, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    freeaddrinfo(address_resource);

    struct sockaddr_storage remote_addr;
    socklen_t remote_addr_s = sizeof remote_addr;
    int accept_desc;

    Timer timer = { 1, 0 };
    ServerState state = {
            init, 0, DATA_SIZE, timer
    };

    char buffer[512];
    long read_size = 0;
    int rcv_file;
    char name_buffer[64];
    int fsize;
    FILE* fptr;
    char buf[4096];
    memset(buf, 0, 4096);
    int retries = 0;
    char* datagram;
    struct Data data;

    while(1) {

        switch (state.status) {
            case init: {


                //get file name
                int init_rcv = rft_rcv(socket_desc, &data);
                FILE_PATH = data.data;

                if(init_rcv < 0){
                    //printf("filename rft_rcv error", errno);
                    return 1;
                }

                read_timeout.tv_sec = 0;
		read_timeout.tv_usec = 10000;

                //send a response back to the client that filepath was recceived, send a 0 if it opened ok, and a -1 if it didn't
                int file_status = 0;
                fptr = fopen(FILE_PATH, "r");
                if (!fptr) {
                    // send this back to client
                    printf("ERROR: Unable to open file for reading stored at %s\n", FILE_PATH);
                    file_status = -1;
                }
                char* nothing = "";
                int path_size = data.SEQ;
                state.expected_ACK = state.expected_ACK + path_size;
                // get size of the file
                fseek(fptr, 0L, SEEK_END);
                fsize = ftell(fptr);
                fseek(fptr, 0L, SEEK_SET);
                rewind(fptr);
                if(file_status == 0) {
                    datagram = make_pkt(data.SEQ, file_status, nothing, IP_SRC, IP_DST, UDP_SRC,
                                           LISTENING_PORT, strlen(test));
                }
                else{
                    datagram = make_pkt(file_status, file_status, nothing, IP_SRC, IP_DST, UDP_SRC,
                                           LISTENING_PORT, strlen(test));
                }

                rft_send(datagram, socket_desc, dest_addr);
                if(file_status == -1){
                    //printf("File unable to be opened\n");
                    return 1;
                }
                start_timer(&state.timer);
              
                state.status = wait_for_ACK;

                break;
            }
            case wait_for_ACK: {
                int bytes_received = rft_rcv(socket_desc, &data);
                if (bytes_received > 0 && (is_ACK(&data, state.expected_ACK))) {
                    if (!is_corrupt(&data)) {
                        printf("Looking for ACK of:%d\n", state.expected_ACK);
                        if (is_ACK(&data, state.expected_ACK)) {
                            state.status = send_data_packets;
                        } else if (is_ACK(&data, -1)) {
                            printf("Error in response - %s\n", data.data);
                        }
                    }
		}else{
                        if(!state.timer.stopped){
                            if(state.timer.num_retries == MAX_RETRIES){
                                printf("Max number of retries reached\n");
                                return 1;
                            }
                            else{
		//		printf("sending %d try\n", state.timer.num_retries);
                                rft_send(datagram, socket_desc, dest_addr);
                                state.timer.num_retries++;
                            }
                        }
                    }
                break;
            }
            case send_data_packets: {
		start_timer(&state.timer);
                if (read_size == fsize) {
                    char * final = "All Data Packets Sent";
                    datagram = make_pkt(-2, -2, final, IP_SRC, IP_DST, UDP_SRC,
                                           LISTENING_PORT, strlen(final));
                    rft_send(datagram, socket_desc, dest_addr);
                    printf("All data successfully sent\n");
                    return 0;
                    //all of the packets have already been sent
                    //send a packet that says we're done?
                    //end the program/restart the entire loop?
                } else if (read_size + DATA_SIZE > fsize) {
                    //this is for when there is less than "512" bytes in the file
                    fread(buffer, 1, (fsize - read_size), fptr);
                    datagram = make_pkt(state.expected_ACK + fsize - read_size, -1, buffer, IP_SRC, IP_DST, UDP_SRC,
                                        LISTENING_PORT, (fsize - read_size));  //12 bytes for the headers
                  //  printf("sending seq of: %d\n", state.expected_ACK + fsize - read_size);


                    int temp = (fsize - read_size);
                    //datagram = create_datagram(buffer);
                    rft_send(datagram, socket_desc, dest_addr);
                    state.expected_ACK = state.expected_ACK + (fsize - read_size);
                    read_size += fsize - read_size;


                } else {
                    fread(buffer, 1, DATA_SIZE, fptr);
                    datagram = make_pkt(state.expected_ACK + DATA_SIZE, -1, buffer, IP_SRC, IP_DST, UDP_SRC,
                                        LISTENING_PORT, DATA_SIZE);  //12 bytes for the headers
                    //printf("sending seq of: %d\n", state.expected_ACK + DATA_SIZE);

                    //datagram = create_datagram(buffer);
                    //call send_packet to send the packet, giving it the datagram
                    read_size += DATA_SIZE;
                    rft_send(datagram, socket_desc, dest_addr);
                    state.expected_ACK = state.expected_ACK + DATA_SIZE;


                }
                state.status = wait_for_ACK;
                break;
            }
            default: {
                // impossible to reach
                return 1;
            }
        }
    }

    return 0;

}


