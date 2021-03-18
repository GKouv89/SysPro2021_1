#ifndef COUNTRY_H
#define COUNTRY_H

typedef struct country{
  char *name;
  unsigned int popByAgeGroup[4];
} Country;

typedef struct pops{
  unsigned int popByAgeGroup[4];  
} population;

struct vaccinations{
  unsigned int inRange;
  unsigned int overall;
};

struct vaccinationsAgeGroup{
  unsigned int inRange[4];
  unsigned int overall[4];
};

Country* create_country(const char *);
void update_population(Country *, int);
int isEqual_country(Country *, unsigned char *);
void print_vaccination_ratios_byAge(Country *, struct vaccinationsAgeGroup *, struct vaccinationsAgeGroup *);
void print_vaccination_ratio(Country *, struct vaccinations *, struct vaccinations *);
void destroy_country(Country **);

#endif