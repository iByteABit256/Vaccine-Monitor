#include "../lib/bloomfilter/bloomfilter.h" 
#include "../lib/skiplist/skiplist.h"
#include "../lib/hashtable/htInterface.h"
#include "../lib/date/date.h"


// Virus
struct virusstr{
    char *name;
    BloomFilter vaccinated_bloom;
    Skiplist vaccinated_persons;
    Skiplist not_vaccinated_persons;
};

typedef struct virusstr *Virus;

// Person
struct personstr{
    char *citizenID;
    char *firstName;
    char *lastName;
    char *country;
    int age;
};

typedef struct personstr *Person;

// Vaccine Record
struct vaccrecstr{
    Person per;
    Date date;
};

typedef struct vaccrecstr *VaccRecord;

// Country Statistics
struct countrystr{
    int population;
    int vaccinated_population;
};

typedef struct countrystr *CountryStatistics;

// Interface
void insertCitizenRecord(VaccRecord, Virus);
void vaccineStatusBloom(char *, Virus);
void vaccineStatus(char *, Virus, HTHash);
void list_nonVaccinated_Persons(Virus, HTHash);
void populationStatus(Virus, Date, Date, char *);
