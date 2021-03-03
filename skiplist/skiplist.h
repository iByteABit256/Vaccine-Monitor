struct node_struct{
    char *key;
    struct node_struct **forward;
};

typedef struct node_struct *skipNode;

skipNode newSkipNode(char *, int);

struct skiplist_struct{
    int maxlvl;
    float p;
    int lvl;
    skipNode header;
};

typedef struct skiplist_struct *Skiplist;

Skiplist newSkiplist(int, float);
int randomLevel(Skiplist);
void skipInsert(Skiplist, char *);
void printSkiplist(Skiplist);

