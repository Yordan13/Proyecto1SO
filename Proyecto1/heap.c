#include <unistd.h>
#include <stdlib.h>
#include "heap.h"

static const unsigned int base_size = 100;

// Prepares the heap for use
void heap_init(struct heap *restrict h, int (*cmpFunc)(PCB,PCB))
{
    compare = cmpFunc;
	*h = (struct heap){
		.size = base_size,
		.count = 0,
		.data = malloc(sizeof(PCB) * base_size)
	};
	if (!h->data) _exit(1); // Exit if the memory allocation fails
}

// Inserts element to the heap
void heap_push(struct heap *restrict h, PCB value)
{
	unsigned int index, parent;

	// Resize the heap if it is too small to hold all the data
	if (h->count == h->size)
	{
		h->size <<= 1;
		h->data = realloc(h->data, sizeof(PCB) * h->size);
		if (!h->data) _exit(1); // Exit if the memory allocation fails
	}

	// Find out where to put the element and put it
	for(index = h->count++; index; index = parent)
	{
		parent = (index - 1) >> 1;
		if (compare(h->data[parent], value)) break;
		h->data[index] = h->data[parent];
	}
	h->data[index] = value;
}

// Removes the biggest element from the heap
void heap_pop(struct heap *restrict h)
{
	unsigned int index, swap, other;

	// Remove the biggest element
	PCB temp = h->data[--h->count];

	// Resize the heap if it's consuming too much memory
	if ((h->count <= (h->size >> 2)) && (h->size > base_size))
	{
		h->size >>= 1;
		h->data = realloc(h->data, sizeof(PCB) * h->size);
		if (!h->data) _exit(1); // Exit if the memory allocation fails
	}

	// Reorder the elements
	for(index = 0; 1; index = swap)
	{
		// Find the child to swap with
		swap = (index << 1) + 1;
		if (swap >= h->count) break; // If there are no children, the heap is reordered
		other = swap + 1;
		if ((other < h->count) && compare(h->data[other], h->data[swap])) swap = other;
		if (compare(temp, h->data[swap])) break; // If the bigger child is less than or equal to its parent, the heap is reordered

		h->data[index] = h->data[swap];
	}
	h->data[index] = temp;
}

// Heapifies a non-empty array
void heapify(PCB data[restrict], unsigned int count)
{
	unsigned int item, index, swap, other;
	PCB temp;

	// Move every non-leaf element to the right position in its subtree
	item = (count >> 1) - 1;
	while (1)
	{
		// Find the position of the current element in its subtree
		temp = data[item];
		for(index = item; 1; index = swap)
		{
			// Find the child to swap with
			swap = (index << 1) + 1;
			if (swap >= count) break; // If there are no children, the current element is positioned
			other = swap + 1;
			if ((other < count) && compare(data[other], data[swap])) swap = other;
			if (compare(temp, data[swap])) break; // If the bigger child is smaller than or equal to the parent, the heap is reordered

			data[index] = data[swap];
		}
		if (index != item) data[index] = temp;

		if (!item) return;
		--item;
	}
}