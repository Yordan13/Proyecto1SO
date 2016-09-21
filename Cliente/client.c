#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include "client.h"

int continuar=1;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[]){
	if (argc==2){
		char* archivo=argv[1];
		leeArchivo(archivo);
	}
	else{
		procesosAleatorios();
	}
}

void procesosAleatorios(){
	int id =1;
	int pausa;
	struct PCB pcb;
	pcb.finish_time=0;
	pcb.waiting_time=0;
	pthread_t tid;
	pthread_create(&tid, NULL, parar, NULL);
	while(continuar){
		pcb.burst=pcb.burstRemaining=obtenerRandom(1,20);
		pcb.priority=obtenerRandom(1,5);
		pcb.id=id;
		sendPCB(pcb);
		pausa=obtenerRandom(1,5);
		sleep(pausa);
		id++;
	}
	pthread_join( tid, NULL);
}
void *parar(){
		printf("Presione una tecla para terminar....");
		getchar();
		continuar=0;
}

void leeArchivo(char* archivo){
		FILE *infile;
		int actualAtributo=1;
		int counter=0;
		struct PCB pcbNuevo;
		char datos[22];
		int pausa;
		memset(datos, 0, 22);
		int caracter;
		infile=fopen(archivo,"r");
		if(infile==NULL){
			printf("Error al abrir el archivo: %s.\n",archivo);
			return ;
		}		
		pcbNuevo.finish_time=0;
		pcbNuevo.arriving_time=0;
		pcbNuevo.waiting_time=0;
		while ((caracter = getc(infile)) != EOF){
			if(caracter==10){	
				pcbNuevo.priority=atoi(datos);
				memset(datos, 0, 22);
				sendPCB(pcbNuevo);
				counter=0;
				actualAtributo=1;
				pausa=obtenerRandom(1,10);
				//sleep(pausa);
				continue;
			}
			if(caracter!=9){
				datos[counter]=(char)caracter;
				counter++;
			}else{
				switch(actualAtributo){
					case 1:	
						pcbNuevo.id=atoi(datos);
						memset(datos, 0, 22);
						actualAtributo++;
						counter=0;
						break;
					case 2:
						pcbNuevo.burst=pcbNuevo.burstRemaining=atoi(datos);
						memset(datos, 0, 22);
						actualAtributo++;
						counter=0;
						break;
				}
				
			}
		}
		fclose(infile);
}

int obtenerRandom(int minimo, int maximo){
  double scaled = (double)rand()/RAND_MAX;
  return (maximo - minimo +1)*scaled + minimo;
}

int sendPCB(PCB pcb)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char* buffer;
    portno = 8080;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    buffer = serialize_PCB(pcb);
    n = write(sockfd,buffer,sizeof(PCB));
    if (n < 0) 
         error("ERROR writing to socket");
    close(sockfd);
    return 0;
}

char* serialize_PCB(PCB pcb){
    char* res = malloc(sizeof(PCB));
    memcpy(res, &pcb, sizeof(PCB));
    return res;
}