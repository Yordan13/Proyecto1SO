#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "circularList.h"

int node_isEmpty(){
   return head == NULL;
}

int node_length(){
  return len;
}
//next element
void node_next(){
	if(current!=NULL)
	current=current->next;
}

//insert link at the first location
void node_insertFirst(struct PCB data){

   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   link->data = data;
	
   if (node_isEmpty()) {
      head = link;
      head->next = head;	  
	  head->previous=head;
	  current=head;
	  len++;
	  
   }else {
      //point it to old first node
	  struct node *temp=head->previous;
	  head->previous=link;
	  link->next= head;
	  link->previous=temp;
	  temp->next=link;
	  len++;
      //point first to new first node
   }    
}

//delete first item
void node_deleteCurrent(){
	int isHead=0;
	if(current==head)isHead=1; 
	if(node_isEmpty())return;
	if(isHead && 
	current->next==current && current->previous==current){
		head=NULL;
		current=NULL;
		len--;
		return NULL;
	}
	struct node *temp;
	temp=current->previous;
	temp->next=current->next;	
	current->next->previous=temp;
	
	len--;
	
	current= temp;
	if(isHead) head=temp;
	node_next();
}

