#include <stdio.h>
#include <stdlib.h>
#include "vaccineMonitor.h"

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
                    per->lastName, per->country, per->age);
        }
    }
}

void populationStatus(Virus vir, Date d1, Date d2, char *country){
    if(country == NULL){
        // No country given
        
    }else{
        // Country given

    }
}
