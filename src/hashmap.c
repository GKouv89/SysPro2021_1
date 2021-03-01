#include <stdio.h>
#include <stdlib.h>
#include "../include/hashmap.h"

void create_map(hashMap **map, int noOfBuckets, typeOfList type){
  (*map) = malloc(sizeof(hashMap));
  (*map)->noOfBuckets = noOfBuckets;
  (*map)->map = malloc(noOfBuckets*sizeof(hashBucket *));
  for(int i = 0; i < (*map)->noOfBuckets; i++){
    (*map)->map[i] = malloc(sizeof(hashBucket));
    create_bucketList(&((*map)->map[i]->bl), type);
  }
}

unsigned long hash_function(hashMap *map, unsigned char *str){
  unsigned long hash = djb2(str);
  return hash % map->noOfBuckets;
}

void insert(hashMap *map, unsigned char *key, void *content){
  unsigned long hash = hash_function(map, key);
  insert_bucketNode(map->map[hash], content);
}

void* find_node(hashMap *map, unsigned char *key){
  unsigned long hash = hash_function(map, key);
  search_bucketList(map->map[hash], key);
}

void destroy_map(hashMap **map){
  for(int i = 0; i < (*map)->noOfBuckets; i++){
    destroy_bucketList(&((*map)->map[i]->bl), (*map)->type);
    free((*map)->map[i]);
    (*map)->map[i] = NULL;
  }
  free((*map)->map);
  (*map)->map = NULL;
  free(*map);
  *map = NULL;
}
