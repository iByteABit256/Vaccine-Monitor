#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/skiplist/skiplist.h"
#include "../lib/bloomfilter/bloomfilter.h"
#include "../lib/hashtable/htInterface.h"
#include "vaccineMonitor.h"
#define MAX_LINE 100
#define MAX_VIRUSES 512

// TODO: error checking 
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

    HTHash citizenRecords = HTCreate();
    HTHash countries = HTCreate();
    HTHash viruses = HTCreate();

    FILE *inputFile = fopen(filePath, "r");

    char *buf = malloc(MAX_LINE*sizeof(char));
    memset(buf, 0, MAX_LINE);

    while(fgets(buf, MAX_LINE, inputFile) != NULL){
        Person per = malloc(sizeof(struct personstr));  
        char *token = strtok(buf, " ");
        
        per->citizenID = malloc((strlen(token)+1)*sizeof(char));
        strcpy(per->citizenID, token);

        token = strtok(NULL, " ");
        per->firstName = malloc((strlen(token)+1)*sizeof(char)); 
        strcpy(per->firstName, token);

        token = strtok(NULL, " ");
        per->lastName = malloc((strlen(token)+1)*sizeof(char)); 
        strcpy(per->lastName, token);

        token = strtok(NULL, " ");
        HTInsert(countries, token, NULL);
        per->countryCode = HTHashFunction(token, countries->curSize);

        token = strtok(NULL, " ");
        per->age = atoi(token);

        HTInsert(citizenRecords, per->citizenID, per);
        
        token = strtok(NULL, " ");
        if(!HTExists(viruses, token)){
            Virus vir = malloc(sizeof(struct virusstr));
            vir->vaccinated_bloom = bloomInitialize(bloomSize);
            vir->vaccinated_persons = newSkiplist(9, 0.5);
            vir->not_vaccinated_persons = newSkiplist(9, 0.5);
        }
    }

    fclose(inputFile);

    return 0;
}

