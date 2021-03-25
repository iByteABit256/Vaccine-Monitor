#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
        fprintf(stderr, "ERROR: INCORRECT ARGUMENTS\n");
        exit(1);
    }

    if(inputFileParameter+1 < argc){
        filePath = argv[inputFileParameter+1];
    }else{
        fprintf(stderr, "ERROR: INCORRECT ARGUMENTS\n");
        exit(1);
    } 

    if(bloomSize+1 < argc){
        bloomSize = atoi(argv[bloomSize+1]);
    }else{
        fprintf(stderr, "ERROR: INCORRECT ARGUMENTS\n");
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

            char *datetok1 = strtok(token, "-\n");
            char *datetok2 = strtok(NULL, "-\n");
            char *datetok3 = strtok(NULL, "-\n");
            if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                date->day = atoi(datetok1); 
                date->month = atoi(datetok2); 
                date->year = atoi(datetok3);
            }else{
                fprintf(stderr, "ERROR: INCORRECT RECORD IN INPUT FILE\n");
                free(date);
                continue;
            }
        }else{
            if(token != NULL){
                fprintf(stderr, "ERROR: INCORRECT RECORD IN INPUT FILE\n");
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

void inputLoop(HTHash countries, HTHash viruses, HTHash citizenRecords, int bloomSize){
    char *buf = malloc(MAX_LINE*sizeof(char));
    memset(buf, 0, MAX_LINE);

    while(1){
        // Read input
        if(fgets(buf, MAX_LINE, stdin) != NULL){

            char *token = strtok(buf, " \n"); 

            if(token == NULL){
                fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                continue;
            }

            // Command name
            if(strcmp(token, "/vaccineStatusBloom") == 0){

                token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                char *citizenID = malloc((strlen(token)+1)*sizeof(char));
                strcpy(citizenID, token);

                token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                if(!HTExists(citizenRecords, citizenID)){
                    fprintf(stderr, "ERROR: NO CITIZEN FOUND WITH ID %s\n\n", citizenID);
                    free(citizenID);
                    continue;
                }

                Virus vir = HTGetItem(viruses, token);

                vaccineStatusBloom(citizenID, vir);
                free(citizenID);

            }else if(strcmp(token, "/vaccineStatus") == 0){

                token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                char *citizenID = malloc((strlen(token)+1)*sizeof(char));
                strcpy(citizenID, token);

                if(!HTExists(citizenRecords, citizenID)){
                    fprintf(stderr, "ERROR: NO CITIZEN FOUND WITH ID %s\n\n", citizenID);
                    free(citizenID);
                    continue;
                }

                token = strtok(NULL, " \n");

                Virus vir = HTGetItem(viruses, token);

                vaccineStatus(citizenID, vir, viruses);
                free(citizenID);

            }else if(strcmp(token, "/populationStatus") == 0){

                char *par1 = strtok(NULL, " \n");
                char *par2 = strtok(NULL, " \n");
                char *par3 = strtok(NULL, " \n");
                char *par4 = strtok(NULL, " \n");

                if(par1 == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                if(!HTExists(countries, par1)){
                    // No country given
                    Virus vir = HTGetItem(viruses, par1);

                    Date d1, d2; 

                    if(par2 == NULL){
                        d1 = NULL;
                    }else{
                        d1 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par2, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d1->day = atoi(datetok1); 
                            d1->month = atoi(datetok2); 
                            d1->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d1);
                            continue;
                        }
                    }

                    if(par3 == NULL){
                        d2 = NULL;
                    }else{
                        d2 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par3, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d2->day = atoi(datetok1); 
                            d2->month = atoi(datetok2); 
                            d2->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d2);
                            continue;
                        }
                    }

                    populationStatus(vir, d1, d2, countries, NULL);

                }else{
                    // Country given

                    if(par2 == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    Virus vir = HTGetItem(viruses, par2);

                    Date d1, d2; 

                    if(par3 == NULL){
                        d1 = NULL;
                    }else{
                        d1 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par3, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d1->day = atoi(datetok1); 
                            d1->month = atoi(datetok2); 
                            d1->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d1);
                            continue;
                        }
                    }

                    if(par4 == NULL){
                        d2 = NULL;
                    }else{
                        d2 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par4, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d2->day = atoi(datetok1); 
                            d2->month = atoi(datetok2); 
                            d2->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d2);
                            continue;
                        }
                    }
                    populationStatus(vir, d1, d2, countries, par1);
                }

            }else if(strcmp(token, "/popStatusByAge") == 0){

                char *par1 = strtok(NULL, " \n");
                char *par2 = strtok(NULL, " \n");
                char *par3 = strtok(NULL, " \n");
                char *par4 = strtok(NULL, " \n");

                if(par1 == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                if(!HTExists(countries, par1)){
                    // No country given
                    Virus vir = HTGetItem(viruses, par1);

                    

                    Date d1, d2; 

                    if(par2 == NULL){
                        d1 = NULL;
                    }else{
                        d1 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par2, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d1->day = atoi(datetok1); 
                            d1->month = atoi(datetok2); 
                            d1->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d1);
                            continue;
                        }
                    }

                    if(par3 == NULL){
                        d2 = NULL;
                    }else{
                        d2 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par3, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d2->day = atoi(datetok1); 
                            d2->month = atoi(datetok2); 
                            d2->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d2);
                            continue;
                        }
                    }

                    popStatusByAge(vir, d1, d2, countries, NULL);

                }else{
                    // Country given

                    if(par2 == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    Virus vir = HTGetItem(viruses, par2);

                    Date d1, d2; 

                    if(par3 == NULL){
                        d1 = NULL;
                    }else{
                        d1 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par3, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d1->day = atoi(datetok1); 
                            d1->month = atoi(datetok2); 
                            d1->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d1);
                            continue;
                        }
                    }

                    if(par4 == NULL){
                        d2 = NULL;
                    }else{
                        d2 = malloc(sizeof(struct datestr));

                        char *datetok1 = strtok(par4, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            d2->day = atoi(datetok1); 
                            d2->month = atoi(datetok2); 
                            d2->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(d2);
                            continue;
                        }

                    }
                    popStatusByAge(vir, d1, d2, countries, par1);
                }

            }else if(strcmp(token, "/insertCitizenRecord") == 0){

                // citizen ID
                token = strtok(NULL, " \n");
                
                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                Person per = HTGetItem(citizenRecords, token);

                if(per == NULL){
                    //  Create new person and insert
                    per = malloc(sizeof(struct personstr));
                    per->citizenID = malloc((strlen(token)+1)*sizeof(char));
                    strcpy(per->citizenID, token);

                    // first name
                    token = strtok(NULL, " \n");

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    per->firstName = malloc((strlen(token)+1)*sizeof(char)); 
                    strcpy(per->firstName, token);

                    // last name
                    token = strtok(NULL, " \n");

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    per->lastName = malloc((strlen(token)+1)*sizeof(char)); 
                    strcpy(per->lastName, token);
                                    
                    // country
                    token = strtok(NULL, " \n");

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

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

                    if(per->age <= 0 || per->age > 120){
                        fprintf(stderr, "ERROR: INVALID AGE\n\n");
                        free(per->citizenID);
                        free(per->firstName);
                        free(per->lastName);
                        free(per);
                        continue;
                    }

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    incrementAgePopulation(country, per->age);


                    HTInsert(citizenRecords, per->citizenID, per);
                }else{
                    // ignore person's information
                    for(int i = 0; i < 4; i++) strtok(NULL, " \n");
                }
                
                // virus
                Virus vir = NULL;

                token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

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

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                char vaccinated = (strcmp(token, "YES") == 0);

                // date
                Date date = NULL;

                token = strtok(NULL, " \n");

                if(vaccinated){
                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    date = malloc(sizeof(struct datestr));

                    char *datetok1 = strtok(token, "-\n");
                        char *datetok2 = strtok(NULL, "-\n");
                        char *datetok3 = strtok(NULL, "-\n");
                        if(datetok1 != NULL && datetok2 != NULL && datetok3 != NULL){
                            date->day = atoi(datetok1); 
                            date->month = atoi(datetok2); 
                            date->year = atoi(datetok3);
                        }else{
                            fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                            free(date);
                            continue;
                        } 
                }else{
                    if(token != NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        exit(1);  
                    } 
                }

                // insert new record
                VaccRecord rec = malloc(sizeof(struct vaccrecstr));
                rec->per = per;
                rec->date = date;

                insertCitizenRecord(rec, vir);

            }else if(strcmp(token, "/vaccinateNow") == 0){

                token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }
                
                Person per = HTGetItem(citizenRecords, token);

                if(per == NULL){
                    //  Create new person and insert
                    per = malloc(sizeof(struct personstr));
                    per->citizenID = malloc((strlen(token)+1)*sizeof(char));
                    strcpy(per->citizenID, token);

                    // first name
                    token = strtok(NULL, " \n");

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    per->firstName = malloc((strlen(token)+1)*sizeof(char)); 
                    strcpy(per->firstName, token);

                    // last name
                    token = strtok(NULL, " \n");

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    per->lastName = malloc((strlen(token)+1)*sizeof(char)); 
                    strcpy(per->lastName, token);
                                    
                    // country
                    token = strtok(NULL, " \n");

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

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

                    if(token == NULL){
                        fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                        continue;
                    }

                    per->age = atoi(token);

                    if(per->age <= 0 || per->age > 120){
                        fprintf(stderr, "ERROR: INVALID AGE\n\n");
                        free(per->citizenID);
                        free(per->firstName);
                        free(per->lastName);
                        free(per);
                        continue;
                    }

                    incrementAgePopulation(country, per->age);

                    HTInsert(citizenRecords, per->citizenID, per);
                }else{
                    // ignore person's information
                    for(int i = 0; i < 4; i++) strtok(NULL, " \n");
                }
                
                // virus
                Virus vir = NULL;

                token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                if(!HTGet(viruses, token, (HTItem *)&vir)){
                    vir = malloc(sizeof(struct virusstr));
                    vir->name = malloc((strlen(token)+1)*sizeof(char)); 
                    strcpy(vir->name, token);
                    vir->vaccinated_bloom = bloomInitialize(bloomSize);
                    vir->vaccinated_persons = newSkiplist(9, 0.5);
                    vir->not_vaccinated_persons = newSkiplist(9, 0.5);

                    HTInsert(viruses, vir->name, vir);
                }

                // date
                Date date = malloc(sizeof(struct datestr));

                time_t t = time(NULL);
                struct tm timestr = *localtime(&t);

                date->day = timestr.tm_mday; 
                date->month = timestr.tm_mon+1; 
                date->year = timestr.tm_year+1900; 

                // insert new record
                VaccRecord rec = malloc(sizeof(struct vaccrecstr));
                rec->per = per;
                rec->date = date;

                VaccRecord old_rec = skipGet(vir->not_vaccinated_persons, per->citizenID);
                if(old_rec){
                    free(old_rec->date);
                    free(old_rec);
                    skipDelete(vir->not_vaccinated_persons, per->citizenID);
                }
                
                insertCitizenRecord(rec, vir);

            }else if(strcmp(token, "/list-nonVaccinated-Persons") == 0){

                char *token = strtok(NULL, " \n");

                if(token == NULL){
                    fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
                    continue;
                }

                Virus v = HTGetItem(viruses, token); 
                list_nonVaccinated_Persons(v, countries);

            }else if(strcmp(token, "/help") == 0){
                printHelp();
            }else if(strcmp(token, "/exit") == 0){
                break;
            }else{
                fprintf(stderr, "ERROR: INCORRECT SYNTAX, SEE /help\n\n");
            }
        }
    }

    free(buf);
}
