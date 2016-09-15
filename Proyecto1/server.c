/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int n;
     int size = sizeof(PCB);
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char* buffer = malloc(size);
     struct sockaddr_in serv_addr, cli_addr;
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 8080;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while(1){
         newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);//acepto la nueva conexion
         if (newsockfd < 0) 
              error("ERROR on accept");
         bzero(buffer,size);
         n = read(newsockfd,buffer,size);
         PCB res = deserialize_PCB(buffer);
         if (n < 0) 
             error("ERROR reading from socket");
         printf("Id: %d\nBurst: %d\nWT: %d\nTAT: %d\nPriority: %d\n\n",res.id,res.burst,res.waiting_time,res.turnaround_time,res.priority);
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}

PCB deserialize_PCB(char* pcb){
    PCB res;
    memcpy(&res, pcb, sizeof(PCB));
    return res;
}