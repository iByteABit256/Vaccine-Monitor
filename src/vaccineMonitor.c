#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vaccineMonitor.h"
#define MIN(a,b) a < b? a : b


// Initialize new country
Country newCountry(char *name){
    Country country = malloc(sizeof(struct countrystr));
    memset(country, 0, sizeof(struct countrystr));
    country->name = name;

    return country;
}

// Increments correct age group population
void incrementAgePopulation(Country country, int age){
    country->agePopulation[MIN(age/20, 3)] += 1;
}

// insert new citizen record related to virus
void insertCitizenRecord(VaccRecord rec, Virus vir){
    if(rec->date == NULL){
        // Citizen is not vaccinated
        skipInsert(vir->not_vaccinated_persons, rec->per->citizenID, rec);
        printf("Inserted %s to not-vaccinated list\n", rec->per->lastName);
    }else{
        // Citizen is vaccinated
        VaccRecord found = skipGet(vir->vaccinated_persons, rec->per->citizenID);
        if(found){
            // Vaccination already recorded
            Person p = found->per;
            Date d = found->date;
            printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %d-%d-%d\n",
                    p->citizenID, d->day, d->month, d->year);
        }else{
            // Citizen is inserted
            skipInsert(vir->vaccinated_persons, rec->per->citizenID, rec);
            bloomInsert(vir->vaccinated_bloom, rec->per->citizenID); 
            printf("Inserted %s to vaccinated list\n", rec->per->lastName);
        }
    }
}

// check if citizen exists in virus bloom filter
void vaccineStatusBloom(char *citizenID, Virus vir){
    if(vir == NULL){
        printf("NO\n");
        return;
    }

    int vaccinated = bloomExists(vir->vaccinated_bloom, citizenID);
    printf("%s\n", vaccinated? "MAYBE" : "NO");
}

// check if citizen exists in virus skiplist
void vaccineStatus(char *citizenID, Virus vir, HTHash viruses){
    if(vir == NULL){
        // no specific virus given, print all
        for(int i = 0; i < viruses->curSize; i++){
            for(Listptr l = viruses->ht[i]->next; l != l->tail; l = l->next){
                Virus vir = ((HTEntry)(l->value))->item;
                char *vname = vir->name;
                VaccRecord rec = skipGet(vir->vaccinated_persons, citizenID);
                if(rec){
                    printf("%s YES %d-%d-%d\n", vname, rec->date->day, 
                           rec->date->month, rec->date->year);
                }else{
                    printf("%s NO\n", vname);
                }
            }
        }

        return;
    }

    // print specific virus info
    VaccRecord rec = skipGet(vir->vaccinated_persons, citizenID);

    if(rec){
        printf("YES %d-%d-%d\n", rec->date->day, rec->date->month, rec->date->year);
    }else{
        printf("NO\n");
    }
}

// prints every non vaccinated person of a specific virus 
void list_nonVaccinated_Persons(Virus v, HTHash countries){
    if(v == NULL){
        printf("No records found\n");
    }else{
        Skiplist skip = v->not_vaccinated_persons;
        
        for(skipNode snode = skip->dummy->forward[0]; 
            snode != NULL; snode = snode->forward[0]){

            Person per = ((VaccRecord)(snode->item))->per;
            printf("%s %s %s %s %d\n", per->citizenID, per->firstName,
                    per->lastName, per->country->name, per->age);
        }
    }
}

// Print function for populationStatus
void printCountryStats(HTHash countries, char *key, HTItem item){
    Country country = item;
    char *name = country->name;
    int vaccPopulation = country->popCounter;
    int population = country->population;
    printf("%s %d %0.2f%%\n", name, vaccPopulation, (float)vaccPopulation/(float)population);

    // set counter back to zero
    country->popCounter = 0;
}

void populationStatus(Virus vir, Date d1, Date d2, HTHash countries, char *countryName){
    // error if only one date was given
    if((d1 == NULL)^(d2 == NULL)){
        fprintf(stderr, "Invalid input\n");
        return;
    }

    int dates_given = 1;

    if(d1 == NULL){
        dates_given = 0;
    }

    if(countryName == NULL){
        Skiplist vacc = vir->vaccinated_persons;
        
        for(skipNode snode = vacc->dummy->forward[0];
            snode != NULL; snode = snode->forward[0]){

            Date date = ((VaccRecord)(snode->item))->date;
            Person per = ((VaccRecord)(snode->item))->per;
            Country country = per->country;

            if(countryName != NULL && strcmp(country->name, countryName)){
                continue;
            }

            if(dates_given){
                if(compareDates(date, d1) >= 0 && compareDates(date, d2) <= 0){
                    country->popCounter += 1;
                }
            }else{
                country->popCounter += 1;
            }
        }
    }
}
