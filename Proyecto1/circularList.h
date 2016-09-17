#include "server.h"
typedef struct node {
   PCB data;
	
   struct node *next;
   struct node *previous;
}Node;

int len;
struct node *head;
struct node *current;

int node_isEmpty();
int node_length();
void node_next();
void node_insertFirst(struct PCB data);
void node_deleteCurrent();
