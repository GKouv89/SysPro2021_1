#include <stdio.h>
#include <stdlib.h>

#include "../include/bloomfilter.h"

unsigned long djb2(unsigned char *str) {
  unsigned long hash = 5381;
  int c;
  while (c = *str++) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

unsigned long sdbm(unsigned char *str) {
  unsigned long hash = 0;
  int c;

  while (c = *str++) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}

unsigned long hash_i(unsigned char *str, unsigned int i) {
  return djb2(str) + i*sdbm(str) + i*i;
}

void create_bloomFilter(bloomFilter **bf, int size, int hf_amt){
  (*bf) = malloc(sizeof(bloomFilter));
  // Because the size will be given to us in bytes
  // we convert it to bits
  (*bf)->hash_func_amount = hf_amt;
  (*bf)->size = 8*size;
  (*bf)->hash_function = &hash_i;
  (*bf)->filter = calloc(size, sizeof(unsigned char));
}

void insert_in_bloomFilter(bloomFilter *bf, unsigned char *str){
  unsigned char mask;
  unsigned long hash;
  int div, mod;
  for(int i = 0; i < bf->hash_func_amount; i++){
    hash = (*(bf->hash_function))(str, i);
    hash = hash % bf->size;
    div = hash/8; // which byte of the filter array are we in?
    mod = hash%8; // which bit of the byte do we wish to change?
    mask = 1;
    mask = mask << (8 - mod - 1);
    bf->filter[div] = bf->filter[div] | mask;
  }
}

int lookup_in_bloomFilter(bloomFilter *bf, unsigned char *str){
  // Returns 1 if str maybe exists in filter
  // 0 if not  
  unsigned char mask;
  unsigned long hash;
  int div, mod, exists = 1;
  for(int i = 0; i < bf->hash_func_amount; i++){
    hash = (*(bf->hash_function))(str, i);
    hash = hash % bf->size;
    div = hash/8; // which byte of the filter array are we in?
    mod = hash%8; // which bit of the byte do we wish to examine?
    mask = 1;
    mask = mask << (8 - mod - 1);
    mask = bf->filter[div] & mask;
    mask = mask >> (8 - mod - 1);
    if(mask == 0){
      exists = 0;
      break;
    }
  }
  return exists;
}

void destroy_bloomFilter(bloomFilter **bf){
  free((*bf)->filter);
  (*bf)->filter = NULL;
  (*bf)->hash_function = NULL;
  free(*bf);
  *bf = NULL;
}
