#include "helpers.h"
#include "state.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// What does checksum do here? Also what data type does it need to be?
unsigned short checksum1(const char *buf, unsigned size)
{
    unsigned sum = 0;
    int i;
    /* Accumulate checksum */
    for (i = 0; i < size - 1; i += 2) {
        unsigned short word16 = *(unsigned short *) &buf[i];
        sum += word16;
    }
    /* Handle odd-sized case */
    if (size & 1) {
        unsigned short word16 = (unsigned char) buf[i];
        sum += word16;
    }
    /* Fold to get the ones-complement result */
    while (sum >> 16) sum = (sum & 0xFFFF)+(sum >> 16);
    /* Invert to get the negative in ones-complement arithmetic */
    return (unsigned short) ~sum;
}

char* make_pkt(int SEQ, int ACK, char* data, char * IP_SRC, char * IP_DST, int UDP_SRC, int UDP_DST, int data_size) {
    char * datagram;
    struct ip ip_header;  //the struct is found in another library, comes with members that we fill in for the ip header standard
    struct udphdr udp_header;
    struct Data data_section;
    data_section.SEQ = SEQ;
    data_section.ACK = ACK;
    memset(data_section.data, 0, (size_t) DATA_SIZE);
    memcpy(data_section.data, data, (size_t) data_size);
    data_section.checksum = (int) checksum1(data_section.data, DATA_SIZE);

    int total_data_size = sizeof(struct Data);

    short total_dram_size = sizeof(ip_header) + sizeof(udp_header) + total_data_size;
    int test = sizeof(ip_header) + sizeof(udp_header);

    struct pseudo_ip_header faked_ip_header_for_checksum;  //made above, must be done in order for UDP checksum to be correctly calculated
    short total_fake_dgram_size = sizeof(ip_header) + sizeof(udp_header) + total_data_size;  //total size of the faked datagram, size of whole packet (change strlen at some point)
    char * fake_dgram_for_udp_checksum; //points to the fake datagram data

    struct in_addr from_ip; //taken from library
    from_ip.s_addr = inet_addr(IP_SRC); //helper function to set the source ip

    struct in_addr to_ip;   //taken from library
    to_ip.s_addr = inet_addr(IP_DST); //helper function to set the dest ip

    ip_header.ip_v = 4;  //ip version (keep as is)
    ip_header.ip_hl = 5; //ip header length, total length of the ip header, it's 5 32-bit words long, (keep this as is)
    ip_header.ip_tos = 0; //type of service, (keep as is)
    ip_header.ip_len = htons(total_dram_size);  //total length of the packet

    //these are all used in packet fragmentation, they will all stay the same because we won't be fragmenting
    ip_header.ip_id = 0; //unique identifier
    ip_header.ip_off = 0; //position of the fragment in the packet
    ip_header.ip_ttl = 255;
    ip_header.ip_p = IPPROTO_UDP;
    ip_header.ip_sum = 0;   //this is the ip checksum, leaving it 0 means that the kernel deals with it
    ip_header.ip_src = from_ip;  //sets the source ip in the packet
    ip_header.ip_dst = to_ip;  //sets the destination ip in the packet


    //this makes the udp headers, there are only 4 fields
    udp_header.uh_sport = htons(UDP_SRC);  //sets the source port, htons changes it from little endian to big endian, has to be done
    udp_header.uh_dport = htons(UDP_DST);  //sets the destination port
    udp_header.uh_ulen = htons(sizeof(udp_header) + total_data_size); //total length of the udp header + udp data, not total length of packet (will want to change strlen)
    udp_header.uh_sum = 0; //checksum

    faked_ip_header_for_checksum.ip_src = ip_header.ip_src;  //this is already in the IP header, so you reuse the value
    faked_ip_header_for_checksum.ip_dst = ip_header.ip_dst;  // ^
    faked_ip_header_for_checksum.unused = 0; //this is just an unused thing for the specification, leave it as it
    faked_ip_header_for_checksum.ip_p = ip_header.ip_p; //the protocol, which is the same as the ip header protocol (set for udp)
    faked_ip_header_for_checksum.uh_ulen = udp_header.uh_ulen; //the length

    fake_dgram_for_udp_checksum = malloc((size_t) total_fake_dgram_size);   //allocate memeory for the fake packet for the checksum
    memset(fake_dgram_for_udp_checksum, 0, (size_t) total_fake_dgram_size); //using memset to clear out the memory (just in case)
    char * cursor = fake_dgram_for_udp_checksum;  //temp cursor to keep track of where you are in the fake packet


    //construction of the fake packet
    memcpy(cursor, &faked_ip_header_for_checksum, sizeof(faked_ip_header_for_checksum)); //put the header in the fake packet arguements:(where we want to put the data, the address of the data, size)

    //put in the real udp headers
    cursor += sizeof(faked_ip_header_for_checksum);  //advance the cursor along the fake packet
    memcpy(cursor, &udp_header, sizeof(udp_header)); //put in the udp header

    cursor += sizeof(udp_header); //move cursor again


    memcpy(cursor, &data_section, total_data_size);

    //now we generate the checksum
    udp_header.uh_sum = checksum1(fake_dgram_for_udp_checksum, total_fake_dgram_size); //generates a checksum for the fake datagram
    free(fake_dgram_for_udp_checksum);  //free up the memory


    //creating the final datagram to be shipped over the socket
    datagram = malloc(total_dram_size);   //allocate memory for the datagram
    memset(datagram, 0, total_dram_size); //clear out the data (just in case)

    cursor = datagram;  //make cursor for filling the datagram
    memcpy(cursor, &ip_header, sizeof(ip_header)); //fill the ip headers

    cursor += sizeof(ip_header);  //move cursor
    memcpy(cursor, &udp_header, sizeof(udp_header));  //fill in the udp header

    cursor += sizeof(udp_header); //move cursor
    memcpy(cursor, &data_section, total_data_size); //put in the data

    //return datagram;
    //printf("total dram:%d\n", total_dram_size);
    return datagram;
}

struct Data* extract_data(char* datagram) {
    struct Data* ptr = (struct Data*) (datagram);
    printf("extracted datagram with... SEQ: %d, ACK: %d, checksum: %d, Data: %s\n\n", ptr->SEQ, ptr->ACK, ptr->checksum, ptr->data);
    return ptr;
}

int rft_rcv(int socket_desc, struct Data* receive_buffer) {
    struct sockaddr addr;
    socklen_t fromlen = sizeof addr;
    struct Data* data;
    char tmp_buffer[sizeof(struct Data)+28];
    int bytes_received = recvfrom(socket_desc, tmp_buffer, sizeof(struct Data)+28, 0, &addr, &fromlen);
    if (bytes_received > 0) {
        data = extract_data(&tmp_buffer[28]);
        memcpy(receive_buffer, data, sizeof(struct Data));
    }
    return bytes_received;
}

int rft_send(char* datagram, int socket_desc, struct sockaddr_in dest_addr) {
    short total_dram_size = 28 + sizeof(struct Data);
    //printf("size of data struct, %d\n", sizeof(struct Data));
    int bytes_sent = sendto(socket_desc, datagram, total_dram_size, 0, (const struct sockaddr *) &dest_addr, sizeof(dest_addr));
    if (bytes_sent < 0 || bytes_sent < total_dram_size) {
        printf("Error sendto(): %s\n", strerror(errno));
        return 0;
    }
    return bytes_sent;
}

int is_corrupt(struct Data* data_section) {
    int checksum = checksum1(data_section->data,DATA_SIZE);
    //printf("Incoming checksum: %d, Calculated checksum: %d\n",data_section->checksum, checksum);
    return data_section->checksum != checksum;
}

int is_ACK(struct Data* data_section, int expected_ACK) {
    return data_section->ACK == expected_ACK;
}

int has_SEQ(struct Data* data_section, int expected_SEQ) {
    return data_section->SEQ == expected_SEQ;
}
