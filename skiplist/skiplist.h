#pragma once

struct node_struct{
    char *key;
    struct node_struct **forward;
};

typedef struct node_struct *skipNode;

skipNode newSkipNode(char *, int);
void destroySkipNode(skipNode);

struct skiplist_struct{
    int maxlvl;
    float p;
    int lvl;
    skipNode dummy;
};

typedef struct skiplist_struct *Skiplist;

Skiplist newSkiplist(int, float);
int coinToss(Skiplist);
void skipInsert(Skiplist, char *);
int skipExists(Skiplist, char *);
void skipDelete(Skiplist, char *);
void skipDestroy(Skiplist);
void skipPrint(Skiplist);

