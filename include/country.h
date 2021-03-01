#ifndef COUNTRY_H
#define COUNTRY_H

typedef struct country{
  char *name;
  unsigned int popByAgeGroup[4];
} Country;

Country* create_country(const char *);
void update_population(Country *, int);
int isEqual_country(Country *, unsigned char *);
void destroy_country(Country **);


#endif