struct bloomstr{
    unsigned char *bloom;
    unsigned int size;
};

typedef struct bloomstr *BloomFilter;

BloomFilter bloomInitialize(unsigned int size); 
unsigned int bloomInsert(BloomFilter bl, char *s);
unsigned int bloomExists(BloomFilter bl, char *s);

