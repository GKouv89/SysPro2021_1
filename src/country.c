#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/country.h"

Country* create_country(const char *name){
  Country *c = malloc(sizeof(Country));
  c->name = malloc((strlen(name) + 1)*sizeof(char));
  strcpy(c->name, name);
  for(int i = 0; i < 4; i++){
    c->popByAgeGroup[i] = 0;
  }
  return c;
}

void update_population(Country *c, int age){
  if(age >= 1 && age < 20){
    c->popByAgeGroup[0]++;
  }else if(age >= 20 && age < 40){
    c->popByAgeGroup[1]++;
  }else if(age >= 40 && age < 60){
    c->popByAgeGroup[2]++;
  }else{
    c->popByAgeGroup[3]++;
  }
}

int isEqual_country(Country *c, unsigned char *str){
  if(strcmp(c->name, str) == 0){
    return 1;
  }else{
    return 0;
  }
}

void destroy_country(Country **c){
  free((*c)->name);
  (*c)->name = NULL;
  free(*c);
  *c = NULL;
}