#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

typedef struct bf{
  int hash_func_amount;
  int size; // size of bloom filter in *bits*
  unsigned long (*hash_function)(unsigned char*, unsigned int);
  unsigned char *filter;
} bloomFilter;

unsigned long djb2(unsigned char *);
unsigned long hash_i(unsigned char *, unsigned int);
void create_bloomFilter(bloomFilter **, int, int);
void insert_in_bloomFilter(bloomFilter *, unsigned char *);
int lookup_in_bloomFilter(bloomFilter *, unsigned char *);
void destroy_bloomFilter(bloomFilter **);

#endif