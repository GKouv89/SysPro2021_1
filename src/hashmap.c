#include <stdio.h>
#include <stdlib.h>
#include "../include/hashmap.h"
#include "../include/bloomfilter.h"

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
  insert_bucketNode(map->map[hash]->bl, content);
}

void* find_node(hashMap *map, unsigned char *key){
  unsigned long hash = hash_function(map, key);
  search_bucketList(map->map[hash]->bl, key);
}

/////////////////////////////////////////////////////////////////
// This function is called only with the viruses hashmap       //
// for the first argument. Basically searches for each element //
// in each bucket, i.e. each virus, whether the citizen        //
// has been vaccinated for it and if so, when.                 //
/////////////////////////////////////////////////////////////////

void lookup_vacStatus_all(hashMap *map, unsigned char *citizenID){
  for(int i = 0; i < map->noOfBuckets; i++){
    vacStatus_all(map->map[i]->bl, citizenID);
  }
}

/////////////////////////////////////////////////////////////////
// This function is called only with the countries hashmap     //
// for the first argument. Basically searched for each country //
// and for one specific virus, how many citizens have been     //
// vaccinated against the virus between the two argument dates //
/////////////////////////////////////////////////////////////////

void lookup_popStatus_all(hashMap *map, int mode, Virus *v, char *startingDate, char *endingDate){
  for(int i = 0; i < map->noOfBuckets; i++){
    popStatus_all(map->map[i]->bl, mode, v, startingDate, endingDate);
  }
}

void destroy_map(hashMap **map){
  for(int i = 0; i < (*map)->noOfBuckets; i++){
    destroy_bucketList(&((*map)->map[i]->bl));
    free((*map)->map[i]);
    (*map)->map[i] = NULL;
  }
  free((*map)->map);
  (*map)->map = NULL;
  free(*map);
  *map = NULL;
}
