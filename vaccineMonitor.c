#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist/skiplist.h"
#include "bloomfilter/bloomfilter.h"

int main(int argc, char *argv[]){

    // Path of input file
    char *filePath;

    // Parameter parsing
    int inputFileParameter = -1;
    int bloomSize = -1;

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i],"-c") == 0){
                inputFileParameter = i;
            }else if(strcmp(argv[i],"-b") == 0){
                bloomSize = i;
            }
        }
    }

    if(inputFileParameter == -1 || bloomSize == -1){
        fprintf(stderr, "Syntax error\n");
        return 1;
    }

    if(inputFileParameter+1 < argc){
        filePath = argv[inputFileParameter+1];
    }else{
        fprintf(stderr, "Syntax error\n");
        return 1;
    } 

    if(bloomSize+1 < argc){
        bloomSize = atoi(argv[bloomSize+1]);
    }else{
        fprintf(stderr, "Syntax error\n");
        return 1;
    }

    FILE *inputFile = fopen(filePath, "r");

    fclose(inputFile);

    return 0;
}
