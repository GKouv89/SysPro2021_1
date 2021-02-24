#ifndef LINKEDLIST_H
#define LINKEDLIST_H

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
void delete_node(list *, int);
boundaries* search(list *, int, listNode *, listNode *, int *);
void destroy_list(list **);
#endif