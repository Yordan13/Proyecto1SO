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
int sendPCB(PCB* pcb);
char* serialize_PCB(PCB pcb);
void leeArchivo(char* archivo);
void procesosAleatorios();
void *parar();
int obtenerRandom(int minimo, int maximo);
void joinThreads(pthread_t idSends[500], int total);
