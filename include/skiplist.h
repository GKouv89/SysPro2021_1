#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "linkedlist.h"

#define p 0.5

typedef struct skiplist{
  int max_height;
  int height;
  list **levels;
} skipList;

void create_skiplist(skipList **, unsigned long long);
char* insert_skipnode(skipList *, int, char *, Citizen *);
void delete_skipnode(skipList *, int);
void print_skiplist(skipList *);
void print_skiplist_citizens(skipList *);
char* search_skip(skipList *, int, listNode *[], int *);
listNode* lookup_skiplist(skipList *s, int);
void print_skiplist_population_of_country(skipList *, int, Country *, char*, char *);
void destroy_skiplist(skipList **);

#endif

