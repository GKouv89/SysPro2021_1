#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "linkedlist.h"

#define p 0.5

typedef struct skiplist{
  int max_height;
  int height;
  list **levels;
} skipList;

void create_skiplist(skipList **, int);
void insert_skipnode(skipList *, int, char *, Citizen *);
void delete_skipnode(skipList *, int);
void print_skiplist(skipList *);
void search_skip(skipList *, int, listNode *[], int *);
void* lookup_skiplist(skipList *, int, int);
void destroy_skiplist(skipList **);

#endif

