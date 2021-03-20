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

        char *token = strtok(buf, " \n");
                
        Person per = HTGetItem(citizenRecords, token);

        if(per == NULL){
            //  Create new person and insert
            per = malloc(sizeof(struct personstr));
            per->citizenID = malloc((strlen(token)+1)*sizeof(char));
            strcpy(per->citizenID, token);

            // first name
            token = strtok(NULL, " \n");
            per->firstName = malloc((strlen(token)+1)*sizeof(char)); 
            strcpy(per->firstName, token);

            // last name
            token = strtok(NULL, " \n");
            per->lastName = malloc((strlen(token)+1)*sizeof(char)); 
            strcpy(per->lastName, token);
                            
            // country
            token = strtok(NULL, " \n");
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
            token = strtok(NULL, " \n");
            per->age = atoi(token);

            incrementAgePopulation(country, per->age);

            HTInsert(citizenRecords, per->citizenID, per);
        }else{
            // ignore person's information
            for(int i = 0; i < 4; i++) strtok(NULL, " \n");
        }
        
        // virus
        Virus vir = NULL;

        token = strtok(NULL, " \n");
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
        token = strtok(NULL, " \n");
        char vaccinated = (strcmp(token, "YES") == 0);

        // date
        Date date = NULL;

        token = strtok(NULL, " \n");

        if(vaccinated){
            date = malloc(sizeof(struct datestr));

            date->day = atoi(strtok(token, "-\n")); 
            date->month = atoi(strtok(NULL, "-\n")); 
            date->year = atoi(strtok(NULL, "-\n")); 
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
    free(buf);
}
