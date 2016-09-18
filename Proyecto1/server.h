#ifndef SERVER_H
#define SERVER_H

typedef struct PCB
{
   int id;
   int burst;
   int burstRemaining;
   int priority;
   int waiting_time;
   int finish_time;
   int arriving_time;
} PCB;

void printQueue();
void printList();
void *consoleThread(char* mode);
PCB deserialize_PCB(char* pcb);
char* serialize_PCB(PCB pcb);
void* initSocket(char* mode);
void* initCpu(void* params);
void scheduleProcess(PCB pcb,char* mode);
void createThreads(char* mode);
int compareSJF(PCB p1, PCB p2);
int compareHPF(PCB p1, PCB p2);
int compareFIFO(PCB p1, PCB p2);
void printPCB(PCB pcb);
void initCpuRR();
#endif