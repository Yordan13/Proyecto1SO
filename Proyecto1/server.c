/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#include "heap.h"
#include "circularList.h"
int ready = 0;
int timeCounter = 0;
int totalProcesses;
int idleTime;
int quantum;
PCB currentProcess;
Heap queue;
PCB completedProcesses[500];
pthread_mutex_t queueMutex;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char* argv[])
{    
    if(argc < 3)
        error("Illegal number of arguments.");
    if(!strcmp("sjf", argv[1]) || !strcmp("hpf", argv[1]) || !strcmp("fifo", argv[1]) 
    || !strcmp("rr", argv[1])){
		if(!strcmp("rr", argv[1]))
		quantum =atoi(argv[3]);
        if(!strcmp("auto", argv[2])){
            createThreads(argv[1]);
            return 0;
        }
        if(!strcmp("man", argv[2])){
            if(argc < 4)
                error("Illegal number of arguments. Must provide a path for manual mode.");
            printf("Man mode. Path: %s", argv[3]);
            return 0;
        }
        else
            error("Illegal argument for programm mode. \nOptions are:\nauto\nman\n");
    }
    else
        error("Illegal argument for algorithm. \nOptions are \nsjf\nhpf\nfifo\nrr");
    return 0; 
}

PCB deserialize_PCB(char* pcb){
    PCB res;
    memcpy(&res, pcb, sizeof(PCB));
    return res;
}

void* initSocket(char* mode){
    int n;
    int size = sizeof(PCB);
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char* buffer = malloc(size);
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("***Error opening socket***");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8080;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
        error("***Error on binding***");
    listen(sockfd,10);
    clilen = sizeof(cli_addr);
    while(!ready);
    while(1){
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);//acepto la nueva conexion
        if (newsockfd < 0) 
            error("***Error on accept***");
        bzero(buffer,size);
        n = read(newsockfd,buffer,size);
        PCB res = deserialize_PCB(buffer);
        if (n < 0) 
            error("***Error reading from socket***");
        scheduleProcess(res,mode);
        usleep(100000);
    }
    free(buffer);
    close(newsockfd);
    close(sockfd);
    pthread_exit(0);
}
void createThreads(char* mode){
    pthread_t jobScheduler;
    pthread_t cpuScheduler;
	pthread_t console;
    if(pthread_create(&jobScheduler, NULL, initSocket, mode) != 0)
        error("***Error creating thread***");
    if(pthread_create(&cpuScheduler, NULL, initCpu, mode) != 0)
        error("***Error creating thread***");
	if(pthread_create(&console, NULL, consoleThread, mode) != 0)
        error("***Error creating thread***");
    pthread_join(jobScheduler, NULL);
    pthread_join(cpuScheduler, NULL);
	pthread_join(console, NULL);
}
void scheduleProcess(PCB pcb, char* mode){
	pthread_mutex_lock(&queueMutex);
	pcb.arriving_time=timeCounter;
	if(strcmp(mode, "rr")==0){
		
		node_insertFirst(pcb);
		
	}else{
		heap_push(&queue, pcb);
	}
	pthread_mutex_unlock(&queueMutex);
}

void *initCpu(void* params){
    char* mode = (char*) params;
    int executing, first;
    executing = 0;
    first = 1;
    if(strcmp(mode, "sjf")==0)//enviar al funcion de ordenamiento correspondiente al heap
        heap_init(&queue,compareSJF);
    else if(strcmp(mode, "hpf")==0)
        heap_init(&queue,compareHPF);
    else if(strcmp(mode, "fifo")==0)
        heap_init(&queue,compareFIFO);
    else if(strcmp(mode, "rr")==0){
        initCpuRR();
		return 0;
	}
    ready = 1;
    printf("CPU Initiated. Mode: %s\n. Press enter to start.", mode);
    while(ready){//loop principal
        if(first){
            first = 0;
            getchar();
        }
        if(!executing){//si hay objetos en la cola y no hay proceso ejecutandose
        pthread_mutex_lock(&queueMutex);//bloquear la cola
			if(queue.count > 0){
				currentProcess = heap_front(&queue);//tomar el proceso del frente
				heap_pop(&queue);
				//currentProcess.arriving_time = timeCounter;
				executing=1;
				printf("\n******Context Switch. Starting new process execution at t=%i*********\n", timeCounter);
				idleTime++;
				pthread_mutex_unlock(&queueMutex);//desbloquear la cola
				continue;
			}
			else{
				printf("\n***Idle Processor***\n");
				idleTime++;
				printf("\nTime: %i\n", timeCounter++);
				pthread_mutex_unlock(&queueMutex);//desbloquear la cola
				sleep(1);
				continue;
			}
			
        }
        else{
			timeCounter++;
			printf("\n***Current Process***\n");
			printPCB(currentProcess);
			printf("\n***Executing Process***\n");
			
			if(--currentProcess.burstRemaining <= 0){//termina la ejecucion.
				executing=0;
				currentProcess.finish_time=timeCounter;
				completedProcesses[totalProcesses++] = currentProcess;
				printf("\n***Ending execution at t=%i***\n",timeCounter);
			}
			
			printf("\nTime: %i\n", timeCounter);
			sleep(1);
		}
    }
    pthread_exit(0);
}

void initCpuRR(){
	int counterActualBurst=0;
	ready = 1;
	int first=1;
	int executing=0;
    printf("CPU Initiated. Round Robin with quantum=%d. Press enter to start.", quantum);
    while(ready){//loop principal
        if(first){
            first = 0;
            getchar();
        }
        if(!executing){//si hay objetos en la cola y no hay proceso ejecutandose
			pthread_mutex_lock(&queueMutex);
			if(node_length() > 0){
				executing=1;
				printf("\n******Context Switch. Starting new process execution at t=%i*********\n", timeCounter);
				printf("******************************************************************\n");
				printPCB(current->data);
				printf("\n******************************************************************");
				printf("\n******************************************************************\n");
				idleTime++;
				pthread_mutex_unlock(&queueMutex);
				sleep(1);
				continue;
			}
			else{
				printf("\n***Idle Processor***\n");
				idleTime++;
				printf("\nTime: %i\n", timeCounter++);
				pthread_mutex_unlock(&queueMutex);
				sleep(1);
				continue;
			}
			
        }
        else{
			current->data.burstRemaining--;
			counterActualBurst++;
			timeCounter++;
			printf("\n***Current Process***\n");
			printPCB(current->data);
			printf("\n***Executing Process***\n");
			if(current->data.burstRemaining <= 0 || counterActualBurst==quantum){//termina la ejecucion.
				executing=0;
				counterActualBurst=0;
				
				printf("\n***Ending actual execution at t=%i*** Remaining Burst=%i",
						timeCounter,current->data.burstRemaining);
				if(current->data.burstRemaining==0){
					current->data.finish_time=timeCounter;
					completedProcesses[totalProcesses++] = current->data;
					node_deleteCurrent();
				}else
				node_next();
			}
			
			printf("\nTime: %i\n", timeCounter);
			sleep(1);
		}
    }
    pthread_exit(0);
}

void *consoleThread(char* mode){
	while(!ready);
	char c;
	while(ready){
		c = getchar();
		if(c == ' '){
			ready = !ready;
			finalReport();
			break;
		}
		if(c == 'q'){
			if(strcmp(mode, "rr")==0){
				printList();
			}else
			printQueue();
		}
	}
	pthread_exit(0);
}

void printList(){
	pthread_mutex_lock(&queueMutex);
	if (head==NULL){
		printf("\n****Queue Empty******\n");
		pthread_mutex_unlock(&queueMutex);		
		return;
	}
	struct node *temp=head;
	printf("\n****Queue******\n");
	printPCB(temp->data);
	while(temp->next!=head){
		printf("\n--------------------\n");
		temp=temp->next;
		printPCB(temp->data);
	}
	printf("\n******************\n");
	pthread_mutex_unlock(&queueMutex);
}

void printQueue(){
	pthread_mutex_lock(&queueMutex);
	printf("\n***Queue***");
	for(int i = 0; i < queue.count; i++)
		printPCB(queue.data[i]);
	printf("\n******************\n");
	pthread_mutex_unlock(&queueMutex);
}

int compareSJF(PCB p1, PCB p2){
	if (p1.burst == p2.burst){
		if (p1.arriving_time == p2.arriving_time)
			return p1.id<p2.id;
		return p1.arriving_time <= p2.arriving_time;
	}
    return p1.burst <= p2.burst;
}
int compareHPF(PCB p1, PCB p2){
	if (p1.priority == p2.priority){
		if (p1.arriving_time == p2.arriving_time)
			return p1.id<p2.id;
		return p1.arriving_time <= p2.arriving_time;
	}
    return p1.priority < p2.priority;
}
int compareFIFO(PCB p1, PCB p2){
	if (p1.arriving_time == p2.arriving_time)
			return p1.id<p2.id;
    return p1.arriving_time <= p2.arriving_time;
}
void printPCB(PCB pcb){
    printf("\n  Id: %i\n Burst: %i\n Remaining: %i\n Priority: %i", 
            pcb.id, pcb.burst, pcb.burstRemaining, pcb.priority);
}
void finalReport(){
		int totalWtime=0;
		int totaltAroundt=0;
		int wTime=0;
		int tAroundt=0;
		struct PCB actual;
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf("****************FINAL REPORT*******************************\n");
		printf("--->Total process executed: %i\n",totalProcesses);
		printf("--->Total idle time: %i\n",idleTime);
		printf("***********************************************************\n");
		for(int i=0; i<totalProcesses;i++){
			actual=completedProcesses[i];
			tAroundt=actual.finish_time-actual.arriving_time;
			wTime=tAroundt-actual.burst;
			totaltAroundt+=tAroundt;
			totalWtime+=wTime;
			printf("--->id: %i\n",actual.id);
			printf("--->Arriving Time: %i\n",actual.arriving_time);
			printf("--->Finish Time: %i\n",actual.finish_time);
			printf("-->Turn Around Time: %i\n",tAroundt);
			printf("-->Waiting Time: %i\n",wTime);			
			printf("------------------------------------------------------------\n");
		}
		printf("***********************************************************\n");
		printf("-->Average Turn Around Time: %9.2f\n",((float)totaltAroundt/(float)totalProcesses));		
		printf("-->Average Waiting Time: %9.2f\n",((float)totalWtime/(float)totalProcesses));
		printf("***********************************************************\n");
}