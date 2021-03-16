#ifndef COUNTRY_H
#define COUNTRY_H

typedef struct country{
  char *name;
  unsigned int popByAgeGroup[4];
} Country;

typedef struct pops{
  unsigned int popByAgeGroup[4];  
} population;

Country* create_country(const char *);
void update_population(Country *, int);
int isEqual_country(Country *, unsigned char *);
void print_vaccination_ratios_byAge(Country *, int[]);
void print_vaccination_ratio(Country *, int[]);
void destroy_country(Country **);


#endif