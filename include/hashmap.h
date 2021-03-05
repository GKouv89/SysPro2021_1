#ifndef HASHMAP_H
#define HASHMAP_H

#include "bucketlist.h"

typedef struct bucket{
  bucketList* bl;
}hashBucket;

typedef struct map{
  int noOfBuckets;
  hashBucket **map;
}hashMap;

void create_map(hashMap **, int, typeOfList);
unsigned long hash_function(hashMap*, unsigned char*);
void insert(hashMap*, unsigned char*, void *);
void* find_node(hashMap*, unsigned char*);
void destroy_map(hashMap**);
// void print_bucket_no_of_entries(hashMap*);
#endif