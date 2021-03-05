#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// #define DEBUG
#include "citizen.h"

typedef struct linkedlistnode{
  int id;
  char *vaccinationDate; // NULL if we are in the not-vaccinated-for skiplist of a virus or
  // on any list in the skiplist that is not the base one
  Citizen *citizen; // NULL if we are in any list in the skiplist that is not the base one
  struct linkedlistnode *prev;
  struct linkedlistnode *next;
  struct linkedlistnode *bottom;
} listNode;

typedef struct bounds{
  listNode *start;
  listNode *end;
} boundaries;

typedef struct linkedlist{
  listNode *front;
  listNode *rear;
} list;

void create_list(list **);
listNode* insert_node(list *, listNode *, int, char *, Citizen *);
void print_list(list *);
listNode* delete_node(list *, int, int, listNode *);
char* search(list *, int, listNode *, listNode *, boundaries **, int *, listNode **);
listNode* cascade(listNode *);
void destroy_list(list **);
#endif