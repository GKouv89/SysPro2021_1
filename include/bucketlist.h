#ifndef BUCKETLIST_H
#define BUCKETLIST_H

typedef enum typeOfList{Country, Virus, Citizen};

typedef struct bucketlistnode{
  void *content;
  struct bucketlistnode *next;
} bucketNode;

typedef struct bucketlist{
  typeOfList type;
  blNode *front;
  blNode *rear;
} bucketList;

void create_bucketList(bucketList **, typeOfList);
void insert_bucketNode(bucketList *, void *);
void print_bucketList(bucketList *);
void* search_bucketList(bucketList *, char *);
void destroy_bucketList(bucketList **);

#endif