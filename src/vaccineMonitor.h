#include "../lib/bloomfilter/bloomfilter.h" 
#include "../lib/skiplist/skiplist.h"
#include "../lib/hashtable/htInterface.h"
#include "../lib/date/date.h"

struct virusstr{
    char *name;
    BloomFilter vaccinated_bloom;
    Skiplist vaccinated_persons;
    Skiplist not_vaccinated_persons;
};

typedef struct virusstr *Virus;

struct personstr{
    char *citizenID;
    char *firstName;
    char *lastName;
    char *country;
    int age;
};

typedef struct personstr *Person;

struct vaccrecstr{
    Person per;
    Date date;
};

typedef struct vaccrecstr *VaccRecord;

void insertCitizenRecord(VaccRecord, Virus);
void vaccineStatusBloom(char *, Virus);
void vaccineStatus(char *, Virus, HTHash);
void list_nonVaccinated_Persons(Virus, HTHash);
void populationStatus(Virus, Date, Date, char *);
