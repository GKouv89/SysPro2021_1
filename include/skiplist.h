#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "linkedlist.h"

typedef struct skiplist{
  int height;
  list **levels;
} skipList;

void create_skiplist(skipList **);
void insert_skipnode(skipList *, int);
void delete_skipnode(skipList *, int);
void search_skip(skipList *, int, listNode ***, int *);
void destroy_skiplist(skipList **);

#endif

