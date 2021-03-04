#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "skiplist/skiplist.h"

int main(void){
    
    srand(time(NULL));

    Skiplist skip = newSkiplist(20, 0.5);

    skipInsert(skip, "Bob");
    skipInsert(skip, "Alice");
    skipInsert(skip, "Jake");
    skipInsert(skip, "Katia");
    skipInsert(skip, "Bop");
    skipInsert(skip, "Alipe");
    skipInsert(skip, "Jape");
    skipInsert(skip, "Kapia");

    skipPrint(skip);

    return 0;
}

