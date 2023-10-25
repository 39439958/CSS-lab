#ifndef __SEND_ICMP_H
#define __SEND_ICMP_H

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)

#define NAMESIZE 10
#define BUFFSIZE 2048
#define key 'K'


typedef struct icmp_tunnel{
    unsigned char sname[NAMESIZE];
    unsigned char dname[NAMESIZE];
    unsigned char data[BUFFSIZE];
}tunnel;

void init_icmp_packet(const char *myname, const char *whoname, const char *dst_ip);
int senddata(char *data);
int filter(struct icmphdr *icmph, int n);
void encrypt(unsigned char *buff);
void decrypt(unsigned char *buff);

#endif