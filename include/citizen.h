#ifndef CITIZEN_H
#define CITIZEN_H

#include "country.h"

typedef struct citizen{
  char *id;
  char *firstName;
  char *lastName;
  int age;
  Country *country;
} Citizen;

Citizen* create_citizen(const char *, const char *, const char *, int, Country *);
int isEqual_citizen(Citizen *, unsigned char *);
int compare_citizens(Citizen *, Citizen *);
void print_citizen(Citizen *);
void destroy_citizen(Citizen **);
#endif