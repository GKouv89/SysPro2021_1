#ifndef VIRUS_H
#define VIRUS_H

#include "skiplist.h"
#include "bloomfilter.h"

typedef struct virus{
  char *name;
  struct skiplist *vaccinated_for;
  struct skiplist *not_vaccinated_for;
  bloomFilter *virusBF;
} Virus;

Virus* create_virus(const char *, unsigned long long, int, int);
void insert_in_virus_bloomFilter(Virus *, unsigned char *);
char* insert_in_vaccinated_for_list(Virus *, int, char *, Citizen *);
void insert_in_not_vaccinated_for_list(Virus *, int, Citizen *);
void remove_from_not_vaccinated_for_list(Virus *, int);
listNode *lookup_in_virus_not_vaccinated_for_list(Virus *, int);
listNode *lookup_in_virus_vaccinated_for_list(Virus *, int);
int lookup_in_virus_bloomFilter(Virus *, unsigned char *);
int isEqual_virus(Virus *, unsigned char *);
void print_non_vaccinated_for(Virus *);
void destroy_virus(Virus **);

#endif