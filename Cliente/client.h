typedef struct PCB
{
   int id;
   int burst;
   int priority;
   int waiting_time;
   int turnaround_time;
} PCB;
int sendPCB(PCB pcb);
char* serialize_PCB(PCB pcb);
void leeArchivo(char* archivo);
void procesosAleatorios();
void *parar();
int obtenerRandom(int minimo, int maximo);