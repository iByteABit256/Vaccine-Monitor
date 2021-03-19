#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vaccineMonitor.h"
#include "parser.h"
#define MAX_LINE 100


// Parses parameters of executable
void parseExecutableParameters(int argc, char *argv[], char **fP, int *bS){

    // Path of input file
    char *filePath;

    // Parameter indices
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
        exit(1);
    }

    if(inputFileParameter+1 < argc){
        filePath = argv[inputFileParameter+1];
    }else{
        fprintf(stderr, "Syntax error\n");
        exit(1);
    } 

    if(bloomSize+1 < argc){
        bloomSize = atoi(argv[bloomSize+1]);
    }else{
        fprintf(stderr, "Syntax error\n");
        exit(1);
    }

    *fP = filePath;
    *bS = bloomSize;
}

void parseInputFile(char *filePath, int bloomSize, HTHash citizenRecords, HTHash countries, HTHash viruses){

    FILE *inputFile = fopen(filePath, "r");

    char *buf = malloc(MAX_LINE*sizeof(char));
    memset(buf, 0, MAX_LINE);

    // Read input file
    while(fgets(buf, MAX_LINE, inputFile) != NULL){
        Person per = malloc(sizeof(struct personstr));  
        char *token = strtok(buf, " ");
        
        // citizen ID
        per->citizenID = malloc((strlen(token)+1)*sizeof(char));
        strcpy(per->citizenID, token);

        // first name
        token = strtok(NULL, " ");
        per->firstName = malloc((strlen(token)+1)*sizeof(char)); 
        strcpy(per->firstName, token);

        // last name
        token = strtok(NULL, " ");
        per->lastName = malloc((strlen(token)+1)*sizeof(char)); 
        strcpy(per->lastName, token);

        // country
        token = strtok(NULL, " ");
        Country country = HTGetItem(countries, token);
        if(country == NULL){
            char *name = malloc((strlen(token)+1)*sizeof(char)); 
            strcpy(name, token);

            country = newCountry(name);
            
            HTInsert(countries, name, country);
        }

        country->population += 1;
        per->country = country;

        // age
        token = strtok(NULL, " ");
        per->age = atoi(token);

        incrementAgePopulation(country, per->age);

        HTInsert(citizenRecords, per->citizenID, per);
        
        // virus
        Virus vir = NULL;

        token = strtok(NULL, " ");
        if(!HTGet(viruses, token, (HTItem *)&vir)){
            vir = malloc(sizeof(struct virusstr));
            vir->name = malloc((strlen(token)+1)*sizeof(char)); 
            strcpy(vir->name, token);
            vir->vaccinated_bloom = bloomInitialize(bloomSize);
            vir->vaccinated_persons = newSkiplist(9, 0.5);
            vir->not_vaccinated_persons = newSkiplist(9, 0.5);

            HTInsert(viruses, vir->name, vir);
        }

        // yes/no
        token = strtok(NULL, " ");
        char vaccinated = (strcmp(token, "YES") == 0);

        // date
        Date date = NULL;

        token = strtok(NULL, " ");

        if(vaccinated){
            date = malloc(sizeof(struct datestr));

            date->day = atoi(strtok(token, "-")); 
            date->month = atoi(strtok(NULL, "-")); 
            date->year = atoi(strtok(NULL, "-")); 
        }else{
            if(token != NULL){
                fprintf(stderr, "Syntax error\n");
                exit(1);  
            } 
        }

        // insert new record
        VaccRecord rec = malloc(sizeof(struct vaccrecstr));
        rec->per = per;
        rec->date = date;

        insertCitizenRecord(rec, vir);
    }

    fclose(inputFile);
}
