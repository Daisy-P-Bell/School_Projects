// helpers.h - helper functions for sending and receiving UDP packets
#ifndef HELPERS_H
#define HELPERS_H

#include <netinet/udp.h>
#include <netinet/ip.h>
#include "state.h"

#define DATA_SIZE 512

struct Data {
    int SEQ;
    int ACK;
    char data[DATA_SIZE];
    unsigned short checksum;
};

struct pseudo_ip_header {  //for calculating UDP checksum, you must have the following five fields
    struct  in_addr ip_src; //source port
    struct  in_addr ip_dst; //destination port
    u_char  unused; // set to zero
    u_char  ip_p;  //protocol
    u_short uh_ulen; //length of the udp header
};

typedef struct Packet Packet;

unsigned short checksum1(const char *buf, unsigned size);
char* make_pkt(int SEQ, int ACK, char* data, char * ip_source, char * ip_dest, int udp_source, int udp_dest, int data_size);
struct Data* extract_data(char* datagram);
int rft_send(char* datagram, int socket_desc, struct sockaddr_in dest_addr);
int rft_rcv(int socket_desc, struct Data* receive_buffer);
int is_corrupt(struct Data* data_section);
int is_ACK(struct Data* data_section, int ACK);
int has_SEQ(struct Data* data_section, int SEQ);

#endif

