#include <stdio.h>
#include <stdlib.h>
#include "vaccineMonitor.h"

void insertCitizenRecord(VaccRecord rec, Virus vir){
    if(rec->date == NULL){
        printf("Inserted %s to not-vaccinated list\n", rec->per->lastName);
    }else{
        printf("Inserted %s to vaccinated list\n", rec->per->lastName);
    }
}

