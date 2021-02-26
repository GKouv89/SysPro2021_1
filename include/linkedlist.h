#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// #define DEBUG

typedef struct linkedlistnode{
  int id;
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
listNode* insert_node(list *, listNode *, int);
void print_list(list *);
listNode* delete_node(list *, int, int, listNode *);
boundaries* search(list *, int, listNode *, listNode *, int *, listNode **);
void destroy_list(list **);
#endif