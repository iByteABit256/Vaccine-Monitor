#include "../lib/bloomfilter/bloomfilter.h" 
#include "../lib/skiplist/skiplist.h"

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
    int countryCode; // Hashed country name
    int age;
};

typedef struct personstr *Person;

struct datestr{
    unsigned int day;
    unsigned int month;
    unsigned int year;
};

typedef struct datestr *Date;

struct vaccrecstr{
    Person per;
    Date date;
};

typedef struct vaccrecstr *VaccRecord;

void insertCitizenRecord(VaccRecord, Virus);
void vaccineStatusBloom(char *, Virus);
void vaccineStatus(char *, Virus);

