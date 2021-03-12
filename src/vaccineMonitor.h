#include "../lib/bloomfilter/bloomfilter.h" 
#include "../lib/skiplist/skiplist.h"

struct virusstr{
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

