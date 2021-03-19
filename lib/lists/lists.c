#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listsTypes.h"

char *ItemFormat = "%d";

Listptr ListCreate(){ 
   Listptr head = malloc(sizeof(Listnode));
   Listptr tail = malloc(sizeof(Listnode));
   head->next = tail;
   head->tail = tail;
   head->head = head;
   tail->head = head;
   tail->tail = tail;
   tail->next = NULL;
   
   return head;
}

int ListSize(Listptr list){
	int size = 0;
	if(list == NULL) return 0;
	while(list->next != list->tail){
		list = list->next;
		size++;
	}
	return size;
}

void ListPrintList(Listptr list){
   if(list == NULL){
      printf("Error: list not initialized\n");
	}else{
		if(list->head->next != list->tail){
			list = list->head->next;
			while(list->next != list->tail){
				printf(ItemFormat, list->value);
				printf(" -> ");
				list = list->next;
			}
			printf(ItemFormat, list->value);
			printf("\n");
		}else printf("List is empty\n");
	}
}

Listptr ListSearch(Listptr list, ItemType item){
   if(list == NULL){
		return NULL;
	}
	if(list->next == list->tail) return NULL;
   else list = list->next;
   while(list->next != list->tail){
      if(item ==  list->value) return list;
      else list = list->next;
	}
	if(item == list->value) return list;
   return NULL;
}

void ListInsertLast(Listptr list, ItemType item){
   if(list == NULL){
      printf("Error: list not initialized\n");
	}
   Listptr new = malloc(sizeof(Listnode));
   new->head = list->head;
   new->tail = list->tail;
	new->next = list->tail;
   new->value = item;
	Listptr temp = list->head;
	while(temp->next != list->tail) temp = temp->next;
	temp->next = new;
}

void ListInsertAfter(Listptr list, ItemType item, Listptr node){
   if(list == NULL){
      printf("Error: list not initialized\n");
	}
	if(node != list->tail){
		Listptr new = malloc(sizeof(Listnode));
		new->head = list->head;
		new->tail = list->tail;
		new->next = node->next;
		new->value = item;
		node->next = new;
	}else{
		printf("Error: Incorrect parameters for insertAfter(Listptr, ItemType, Listptr)\n");
	}
}

void ListDeleteLast(Listptr list){
	if(list == NULL){
      printf("Error: list not initialized\n");
	}else{
		if(list->head->next == list->tail){
			printf("List is empty\n");
		}else{      
			Listptr temp = list->head; 
			while(temp->next->next != temp->tail) temp = temp->next;
			Listptr last = temp->next;
			temp->next = temp->tail;
			free(last);
		}
	}
}

void ListDelete(Listptr list, Listptr node){
	if(list != NULL){
		if(node == list->head || node == list->tail) printf("Error: Incorrect parameters for delete(Listptr, Listptr)\n");
		else{
			while(list->next != node && list->next != list->tail){
				list = list->next;
			}   
			if(list->next != list->tail){
				list->next = node->next;
				free(node);
			}
		}
	}else{
		printf("Error: list not initialized\n");
	}
}

void ListDestroy(Listptr list){
	if(list == NULL) return;
	Listptr next;
	for(Listptr l = list->head; l != NULL; l = next){
		next = l->next;
		free(l);
	}
}

Listptr ListGetNext(Listptr list){
	if(list == NULL || list->next == list->tail) return NULL;
	return list->next;
}

Listptr ListGetFirst(Listptr list){
	if(list == NULL) return NULL;
	list = list->head;
	if(list->next != list->tail) return list->next;
	else return NULL;
}

Listptr ListGetLast(Listptr list){
	if(list == NULL) return NULL;
	Listptr l;
	for(l = list->head; l->next != l->tail; l = l->next);
	if(l == list->head) return NULL;
	else return l;
}

