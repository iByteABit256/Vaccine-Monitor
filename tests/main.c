#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloomfilter/bloomfilter.h"

int main(void){
    
    BloomFilter b = bloomInitialize(10000);

    bloomInsert(b, "John Frusciante"); 

    if(bloomExists(b, "John Frusciante")){
        printf("Maybe\n");
    }else{
        printf("No\n"); 
    }

    if(bloomExists(b, "Paul Gilbert")){
        printf("Maybe\n");
    }else{
        printf("No\n"); 
    }

    bloomInsert(b, "Paul Gilbert");

    if(bloomExists(b, "John Frusciante")){
        printf("Maybe\n");
    }else{
        printf("No\n"); 
    }

    if(bloomExists(b, "Paul Gilbert")){
        printf("Maybe\n");
    }else{
        printf("No\n"); 
    }

    bloomDestroy(b);

    free(b);

    return 0;
}

