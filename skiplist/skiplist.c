#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist.h"

// Constructs new skip node
skipNode newSkipNode(char *key, int lvl){
    skipNode node = malloc(sizeof(struct node_struct)); 

    // Pass by reference to save memory
    node->key = key;

    node->forward = malloc(sizeof(skipNode *)*(lvl+1));
    memset(node->forward, 0, sizeof(skipNode *)*(lvl+1));

    return node;
}

// Constructs new skip list with given maximum level and
// coin toss heads probability
Skiplist newSkiplist(int maxlvl, float p){
    Skiplist sl = malloc(sizeof(struct skiplist_struct));

    sl->maxlvl = maxlvl;
    sl->p = p;
    sl->lvl = 0;
    sl->header = newSkipNode(NULL, maxlvl);    

    return sl;
}

// Random amount of levels for skiplist node
int randomLevel(Skiplist sl){
    // Coin toss
    float toss = (float)rand()/(float)RAND_MAX;

    int lvl = 0;

    // Toss until failure or max level reached
    while(toss < sl->p && lvl < sl->maxlvl){
        lvl++;
        float toss = (float)rand()/(float)RAND_MAX;
    }

    return lvl;
}

// Insert new node to skiplist
void skipInsert(Skiplist sl, char *key){

    // Current node
    skipNode curr = sl->header; 

    // Directory that keeps track of each level 
    skipNode dir[sl->maxlvl+1];
    memset(dir, 0, sizeof(skipNode)*(sl->maxlvl+1));

    for(int i = sl->lvl; i >= 0; i--){
        while(curr->forward[i] != NULL && strcmp(curr->forward[i]->key, key) < 0){
            curr = curr->forward[i]; 
        }
        dir[i] = curr;
    }

    // Check next node of level 0
    curr = curr->forward[0];

    // If end of level, or before a different node then it doesn't exist
    if(curr == NULL || strcmp(curr->key, key)){

        // Start coin tossing to determine node level
        int rand_level = randomLevel(sl);

        if(rand_level > sl->lvl){
            for(int i = sl->lvl+1; i <= rand_level; i++){
                dir[i] = sl->header;
            }

            sl->lvl = rand_level;
        }

        // New node
        skipNode node = newSkipNode(key, rand_level);

        // Update directory and forward nodes
        for(int i = 0; i <= rand_level; i++){
            node->forward[i] = dir[i]->forward[i];
            dir[i]->forward[i] = node;
        }
    }
}

// Prints skiplist
void printSkiplist(Skiplist sl){

    /*
     * Format:
     *
     * Ln  : a0 -> a1 -> ... -> an
     * Ln-1: b0 -> b1 -> ... -> bn
     *             ...
     * L0  : c0 -> c1 -> ... -> cn
     *
    */

    for(int i = sl->lvl; i >= 0; i--){
        skipNode node = sl->header->forward[i];
        
        while(node != NULL){
            printf("%s", node->key);
            node = node->forward[i];

            if(node != NULL){
                printf("-> ");
            }
        }
        printf("\n");
    }

    printf("\n");
}

