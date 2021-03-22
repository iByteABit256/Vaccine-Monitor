#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htInterface.h"
#define defSize 65
#define maxWord 40

int HTHashFunction(char *key, int size){
	int res = 0, i = 0;
	while(key[i] != '\0'){
		res += key[i++];
	}
	return res%size;
}

Listptr ListSearchKey(Listptr list, char *item){
   if(list == NULL){
		return NULL;
	}
	if(list->next == list->tail) return NULL;
   if(list->next != NULL) list = list->next;
   while(list->next != list->tail){
      if(!strcmp(item, ((HTEntry)(list->value))->key)) return list;
      else list = list->next;
	}
	if(!strcmp(item, ((HTEntry)(list->value))->key)) return list;
   return NULL;
}

HTHash HTCreate(void){
	HTHash hash = malloc(sizeof(struct hthash));
	hash->ht = malloc(sizeof(Listptr)*defSize);
	for(int i = 0; i < defSize; i++){
		hash->ht[i] = ListCreate();
	}
	hash->curSize = defSize;
	return hash;
}

void HTRehash(HTHash hash){
	HTEntry *entries = malloc(sizeof(struct hashentry)*hash->curSize);
	int n = 0;
	for(int i = 0; i < hash->curSize; i++){
		for(Listptr l = hash->ht[i]->next; l != l->tail;){
			Listptr temp = l->next;
			entries[n++] = l->value;
			l->value = NULL;
			ListDelete(l->head, l);
			l = temp;
		}
	}
	hash->curSize *= 2;
	for(int i = 0; i < n; i++){
		char *key = entries[i]->key;
		HTItem item = entries[i]->item;
		free(entries[i]);
		HTInsert(hash, key, item);
	}
	free(entries);
}

HTHash HTInsert(HTHash hash, char *key, HTItem item){
	HTEntry entry = malloc(sizeof(struct hashentry));
	entry->key = key;
	entry->item = item;
	int index = HTHashFunction(key, hash->curSize);
	Listptr list = ListSearchKey(hash->ht[index], key);	
	if(list == NULL){
		ListInsertLast(hash->ht[index], entry);
	}else{
		((HTEntry)list->value)->item = item;
		free(entry);
		return NULL;
	}
	double loadratio = (float)HTSize(hash)/(float)hash->curSize;
	if(loadratio >= 0.9){
		//free(entry);
		hash->ht = realloc(hash->ht, sizeof(Listptr)*2*hash->curSize);
		for(int i = hash->curSize; i < 2*hash->curSize; i++){
			hash->ht[i] = ListCreate();	
		}
		HTRehash(hash);
	}
	return hash;
}

int HTSize(HTHash hash){
	if(hash->curSize == 0) return 0;
	int size = 0;
	for(int i = 0; i < hash->curSize; i++){
		size += ListSize(hash->ht[i]);	
	}
	return size;
}

int HTGet(HTHash hash, char *key, HTItem *itemptr){
    if(key == NULL){
        return 0;
    }

	int index = HTHashFunction(key, hash->curSize);
	Listptr list = ListSearchKey(hash->ht[index], key);
	if(list){
		*itemptr = ((HTEntry)(list->value))->item;
		return 1;
	}else{
		return 0;
	}
}

int HTExists(HTHash hash, char *key){
	int index = HTHashFunction(key, hash->curSize);
	Listptr list = ListSearchKey(hash->ht[index], key);

    return list != NULL;
}

void HTRemove(HTHash hash, char *key){
	int index = HTHashFunction(key, hash->curSize);
	Listptr list = ListSearchKey(hash->ht[index], key);	
	if(list){
		free(list->value);
		ListDelete(list->head, list);
	}
}

void HTVisit(HTHash hash, void (*visit)(HTHash, char *, HTItem)){
	for(int i = 0; i < hash->curSize; i++){
		for(Listptr l = hash->ht[i]->next; l != l->tail; l = l->next){
			visit(hash, ((HTEntry)(l->value))->key, ((HTEntry)(l->value))->item);	
		}
	}
}

void HTDestroy(HTHash hash){
	for(int i = 0; i < hash->curSize; i++){
		Listptr l = (Listptr)hash->ht[i]->next;
		for(; l != (Listptr)l->tail;){
			Listptr temp = l->next;
			//free(((HTEntry)l->value)->key);
			//free(((HTEntry)l->value)->item);
			free(l->value);
			ListDelete(l->head, l);	
			l = temp;
		}
		free(l->head);
		free(l->tail);
	}
	free(hash->ht);
	hash->curSize = 0;
	free(hash);
}

HTItem HTGetItem(HTHash hash, char *key){
    if(key == NULL){
        return NULL;
    }

	int index = HTHashFunction(key, hash->curSize);
	Listptr list = ListSearchKey(hash->ht[index], key);
	if(list){
		return ((HTEntry)(list->value))->item;
	}else{
		return NULL;
	}
}

