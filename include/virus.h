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

Virus* create_virus(const char *, int, int, int);
int isEqual_virus(Virus *, unsigned char *);
void destroy_virus(Virus **);

#endif