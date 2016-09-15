typedef struct PCB
{
   int id;
   int burst;
   int priority;
   int waiting_time;
   int turnaround_time;
} PCB;

PCB deserialize_PCB(char* pcb);
char* serialize_PCB(PCB pcb);