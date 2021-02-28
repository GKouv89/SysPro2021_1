#ifndef ENTITIES_H
#define ENTITIES_H

#include "skiplist.h"
#include "bloomfilter.h"

typedef struct country{
  char *name;
  unsigned int popByAgeGroup[4];
} Country;

typedef struct virus{
  char *name;
  skipList *vaccinated_for;
  skipList *not_vaccinated_for;
  bloomFilter *virusBF;
} Virus;

typedef struct citizen{
  char *id;
  char *firstName;
  char *lastName;
  int age;
  Country *country;
} Citizen;

Country* create_country(const char *);
Virus* create_country(const char *);
Citizen* create_citizen(const char *, const char *, const char *, int, Country *);
int isEqual_country(Country *, unsigned char *);
int isEqual_virus(Virus *, unsigned char *);
int isEqual_citizen(Citizen *, unsigned char *);
void destroy_country(Country **);
void destroy_virus(Virus **);
void destroy_citizen(Citizen **);

#endif