#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "udp_sock_proto.h"

#define MAXBUF 3


  struct sockaddr_storage server_storage;
  struct math_proto mp; 
  struct math_proto mp_result;
  // struct sockaddr_in udp_client_addr;
  // socklen_t client_addr_size;

int ReceivedData(){
  int len = recvfrom(sockfd, buf, MAXBUF, 0, (struct sockaddr *)&server_storage, &addr_size); //receive data from client 
  printf("Received from client: %d bytes\n", len);
  for(int i=0;i<len;i++) { printf("%02x ", buf[i]); } printf("\n"); //print received data dump
  return len;
}

int main() {
  if (setAll(MAXBUF) == 0) return 0;

  bind(sockfd, (struct sockaddr *) &udp_addr, sizeof(udp_addr)); // bind the socket

  int len = ReceivedData();

  //check valid ??
  if(len==3) {	
    math_proto_parse_pkt(&mp, buf); 
    math_proto_print_debug(&mp);

   	math_proto_calculate(&mp, &mp_result);
    math_proto_print_debug(&mp_result);
   	
   	math_proto_create_pkt(&mp_result, buf);
   	
		sendto(sockfd, buf, 3, 0, (struct sockaddr *)&server_storage, addr_size); //send the data to client
		
  }

  return 0;
}
