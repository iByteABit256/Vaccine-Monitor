#include "htTypes.h"
int HTHashFunction(char *, int);
HTHash HTCreate(void);
HTHash HTInsert(HTHash, char *, HTItem);
int HTSize(HTHash);
int HTGet(HTHash, char *, HTItem *);
void HTRemove(HTHash, char *);
void HTVisit(HTHash, void (*visit)(HTHash, char *, HTItem));
void HTDestroy(HTHash);
HTItem HTGetItem(HTHash, char *);
