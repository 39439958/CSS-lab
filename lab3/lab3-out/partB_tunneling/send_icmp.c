#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include<arpa/inet.h>

#include "send_icmp.h"

extern unsigned char sendbuff[BUFFSIZE];
extern unsigned char recvbuff[BUFFSIZE];

extern tunnel *tnel;
extern int socket_id;
extern struct sockaddr_in dest_addr;
extern unsigned short total_len;

static unsigned short icmp_checksum(
                unsigned short *ptr, int nbytes) {
    long sum;
    unsigned short oddbyte;
    unsigned short answer;
    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((uint8_t *) &oddbyte) = *(uint8_t *)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}


void init_icmp_packet(const char *myname,
                     const char *whoname, const char *dst_ip) {
    tnel = (tunnel *)malloc(sizeof(tunnel));
    memcpy(tnel->sname, myname, strlen(myname));
    memcpy(tnel->dname, whoname, strlen(whoname));
    struct icmphdr *icmph = (struct icmphdr *)(sendbuff);

    icmph->code = 0;
    icmph->un.echo.id = htons(1);
    icmph->un.echo.sequence = htons(1);
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, dst_ip, &dest_addr.sin_addr);
}

int senddata(char *data){
    // Exercise 2.
    // Add your code here:
    tunnel *sendt = (tunnel *)(sendbuff + sizeof(struct icmphdr));
    struct icmphdr *icmph = (struct icmphdr *)(sendbuff);
    
    icmph->type = ICMP_ECHO;
    strcpy(sendt->dname,tnel->dname);
    strcpy(sendt->sname,tnel->sname);
    strcpy(sendt->data,tnel->data);

    // total_len += 10;
    // unsigned short checksum = icmp_checksum((unsigned short *)sendbuff, total_len);
    // icmph->checksum = checksum;
    encrypt(sendbuff);

    int bytes_sent = sendto(socket_id, sendbuff, sizeof(sendbuff), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (bytes_sent == -1) {
        perror("ICMP packet send failed");
        close(socket_id);
        exit(1);
    }

    return 0;
}

int filter(struct icmphdr *icmph, int n){
    tunnel *recv = (tunnel *)(recvbuff + sizeof(struct iphdr)
                                      + sizeof(struct icmphdr)); 

    // total_len = n - sizeof(struct iphdr); // sub the header of ip
    // unsigned short checksum = icmph->checksum;
    // icmph->checksum = 0;

    if (icmph->type != ICMP_ECHO ||
        (strcmp(recv->dname, tnel->sname) != 0)) 
        return 0;
    return 1;
}

void encrypt(unsigned char *buff){
    for (int i = 0; i < sizeof(buff); i++) {
        buff[i] = buff[i] ^ key;
    }
}

void decrypt(unsigned char *buff){
    for (int i = 0; i < sizeof(buff); i++) {
        buff[i] = buff[i] ^ key;
    }
}