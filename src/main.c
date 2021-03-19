#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/skiplist/skiplist.h"
#include "../lib/bloomfilter/bloomfilter.h"
#include "../lib/hashtable/htInterface.h"
#include "vaccineMonitor.h"
#include "parser.h"
#define MAX_LINE 100
#define MAX_VIRUSES 512


// TODO: error checking 
int main(int argc, char *argv[]){

    char *filePath;
    int bloomSize;

    parseExecutableParameters(argc, argv, &filePath, &bloomSize);

    HTHash citizenRecords = HTCreate();
    HTHash countries = HTCreate();
    HTHash viruses = HTCreate();

    parseInputFile(filePath, bloomSize, citizenRecords, countries, viruses);

    char *buf = malloc(MAX_LINE*sizeof(char));
    memset(buf, 0, MAX_LINE);

    while(1){
        // Read input
        if(fgets(buf, MAX_LINE, stdin) != NULL){

            char *token = strtok(buf, " \n"); 

            if(token == NULL){
                continue;
            }

            // Command name
            if(strcmp(token, "/vaccineStatusBloom") == 0){

                token = strtok(NULL, " \n");

                char *citizenID = malloc((strlen(token)+1)*sizeof(char));
                strcpy(citizenID, token);

                token = strtok(NULL, " \n");

                Virus vir = HTGetItem(viruses, token);

                vaccineStatusBloom(citizenID, vir);
                free(citizenID);

            }else if(strcmp(token, "/vaccineStatus") == 0){

                token = strtok(NULL, " \n");

                char *citizenID = malloc((strlen(token)+1)*sizeof(char));
                strcpy(citizenID, token);

                token = strtok(NULL, " \n");
                
                Virus vir = HTGetItem(viruses, token);

                vaccineStatus(citizenID, vir, viruses);
                free(citizenID);

            }else if(strcmp(token, "/populationStatus") == 0){

                char *par1 = strtok(NULL, " \n");
                char *par2 = strtok(NULL, " \n");
                char *par3 = strtok(NULL, " \n");
                char *par4 = strtok(NULL, " \n");

                if(par4 == NULL){
                    // No country given
                    Virus vir = HTGetItem(viruses, par1);

                    Date d1, d2; 

                    d1 = malloc(sizeof(struct datestr));

                    d1->day = atoi(strtok(par2, "-\n")); 
                    d1->month = atoi(strtok(NULL, "-\n")); 
                    d1->year = atoi(strtok(NULL, "-\n")); 

                    d2 = malloc(sizeof(struct datestr));

                    d2->day = atoi(strtok(par3, "-\n")); 
                    d2->month = atoi(strtok(NULL, "-\n")); 
                    d2->year = atoi(strtok(NULL, "-\n")); 

                    populationStatus(vir, d1, d2, countries, NULL);

                }else{
                    // Country given
                    Virus vir = HTGetItem(viruses, par2);

                    Date d1, d2; 

                    d1 = malloc(sizeof(struct datestr));

                    d1->day = atoi(strtok(par3, "-\n")); 
                    d1->month = atoi(strtok(NULL, "-\n")); 
                    d1->year = atoi(strtok(NULL, "-\n")); 

                    d2 = malloc(sizeof(struct datestr));

                    d2->day = atoi(strtok(par4, "-\n")); 
                    d2->month = atoi(strtok(NULL, "-\n")); 
                    d2->year = atoi(strtok(NULL, "-\n")); 

                    populationStatus(vir, d1, d2, countries, par1);

                }

            }else if(strcmp(token, "/popStatusByAge") == 0){

            }else if(strcmp(token, "/insertCitizenRecord") == 0){

                Person per = malloc(sizeof(struct personstr));  
                token = strtok(NULL, " \n");
                
                // citizen ID
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

            }else if(strcmp(token, "/vaccinateNow") == 0){

                Person per = malloc(sizeof(struct personstr));  
                token = strtok(NULL, " \n");
                
                // citizen ID
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

                skipDelete(vir->not_vaccinated_persons, per->citizenID);
                insertCitizenRecord(rec, vir);

            }else if(strcmp(token, "/list-nonVaccinated-Persons") == 0){

                char *token = strtok(NULL, " \n"); 
                Virus v = HTGetItem(viruses, token); 
                list_nonVaccinated_Persons(v, countries);

            }else if(strcmp(token, "/exit") == 0){

                break;

            }else{

                printf("Invalid command\n");

            }
        }
    }

    return 0;
}
