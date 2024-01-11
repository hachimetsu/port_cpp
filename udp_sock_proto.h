#ifndef _UDP_SOCK_PROTO_H_
#define _UDP_SOCK_PROTO_H_
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio_ext.h>

int sockfd;
unsigned char *buf;
struct sockaddr_in udp_addr;
socklen_t addr_size;


int setAll(int size){
	buf = (unsigned char *)malloc(size * (sizeof(unsigned char)));
	sockfd = socket(PF_INET, SOCK_DGRAM, 0); // create a UDP socket
  	if(sockfd<=0) { printf("socket error !\n"); return 0; }
  
	/* configure settings to communicate with remote UDP server */
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = htons(6810);				   // server port
	udp_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // local-host
	memset(udp_addr.sin_zero, '\0', sizeof udp_addr.sin_zero);
	addr_size = sizeof udp_addr;
	return 1;
}

struct math_proto { unsigned char opr; unsigned char val1; unsigned char val2; };


//Manual parsing Rx (Pkt -> DS)
void math_proto_parse_pkt(struct math_proto *mp, unsigned char *buf)
{	mp->opr = buf[0]; mp->val1 = buf[1]; mp->val2=buf[2]; }

//Automating parsing
void math_proto_parse_pkt2(struct math_proto *mp, unsigned char *buf)
{	memcpy(mp, buf, sizeof(struct math_proto)); }


//To Send Tx (DS -> Pkt)
void math_proto_create_pkt(struct math_proto *mp, unsigned char *buf)
{	buf[0] = mp->opr; buf[1] = mp->val1; buf[2] = mp->val2; }

//Actual operation or can be some FSM, ... etc.
void math_proto_calculate(struct math_proto *mp, struct math_proto *mp2)
{	unsigned char result=0;
	if(mp->opr=='+') { result = mp->val1+mp->val2; }
	else if(mp->opr=='-') { result = mp->val1-mp->val2; }

	mp2->opr = '='; mp2->val1 = result; mp2->val2 = 0x00;
}

//Debug/Dump the DS
void math_proto_print_debug(struct math_proto *mp)
{	printf("opr=%02x, val1=%02x, val2=%02x\n", mp->opr, mp->val1, mp->val2); }

#endif
