#include "server.h"
typedef struct heap
{
	unsigned int size; // Size of the allocated memory (in number of items)
	unsigned int count; // Count of the elements in the heap
	PCB *data; // Array with the elements
} Heap;

void heap_init(struct heap *restrict h, int (*cmpFunc)(PCB,PCB));
void heap_push(struct heap *restrict h, PCB value);
void heap_pop(struct heap *restrict h);
int (*compare)(PCB,PCB);

// Returns the biggest element in the heap
#define heap_front(h) (*(h)->data)

// Frees the allocated memory
#define heap_term(h) (free((h)->data))

void heapify(PCB data[restrict], unsigned int count);