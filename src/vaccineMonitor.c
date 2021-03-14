#include <stdio.h>
#include <stdlib.h>
#include "vaccineMonitor.h"

void insertCitizenRecord(VaccRecord rec, Virus vir){
    if(rec->date == NULL){
        // Citizen is not vaccinated
        skipInsert(vir->not_vaccinated_persons, rec->per->citizenID, rec->per);
        printf("Inserted %s to not-vaccinated list\n", rec->per->lastName);
    }else{
        // Citizen is vaccinated
        skipInsert(vir->vaccinated_persons, rec->per->citizenID, rec);
        bloomInsert(vir->vaccinated_bloom, rec->per->citizenID); 
        printf("Inserted %s to vaccinated list\n", rec->per->lastName);
    }
}

void vaccineStatusBloom(char *citizenID, Virus vir){
    int vaccinated = bloomExists(vir->vaccinated_bloom, citizenID);
    printf("%s\n", vaccinated? "MAYBE" : "NO");
}

void vaccineStatus(char *citizenID, Virus vir){
    VaccRecord rec = skipGet(vir->vaccinated_persons, citizenID);

    if(rec){
        printf("YES %d-%d-%d\n", rec->date->day, rec->date->month, rec->date->year);
    }else{
        printf("NO\n");
    }
}

