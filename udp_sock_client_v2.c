#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include "udp_sock_proto.h"

#define MAXBUF 3

struct math_proto mp;	
void sendData(unsigned char opr, unsigned char val1, unsigned char val2){
  mp.opr = opr;
  mp.val1 = val1;
  mp.val2 = val2;
  math_proto_print_debug(&mp);
  math_proto_create_pkt(&mp, buf);
  sendto(sockfd, buf, MAXBUF, 0, (struct sockaddr *)&udp_addr, addr_size); // send the data to server
}
int ReceivedData() {
  int len = recvfrom(sockfd, buf, MAXBUF, 0, NULL, NULL); // receive data from server
  printf("Received from server: %d bytes\n", len);
  for(int i=0;i<len;i++) { printf("%02x", buf[i]); } printf("\n"); //print received data dump
  return len;
}

int main(){
  if (setAll(MAXBUF) == 0) return 0;
  
  sendData('+', 0x01, 0x02);
  int len = ReceivedData();

  //parsing back to the data-structure
  //check valid ??
  if(len==3) { 
    math_proto_parse_pkt(&mp, buf); 
    math_proto_print_debug(&mp); 
  }	

  	
  close(sockfd); //close socket file-descriptor
  
  return 0;
}
