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

void print_vaccination_ratios_byAge(Country *c, int vaccinated[]){
  printf("%s\n", c->name);
  if(c->popByAgeGroup[0] == 0){
    printf("0-20 0 0\n");
  }else{
    printf("0-20 %d %.2lf\n", vaccinated[0], ((double)vaccinated[0]/(double)c->popByAgeGroup[0])*100);
  }
  if(c->popByAgeGroup[1] == 0){
    printf("21-40 0 0\n");
  }else{
    printf("21-40 %d %.2lf\n", vaccinated[1], ((double)vaccinated[1]/(double)c->popByAgeGroup[1])*100);
  }
  if(c->popByAgeGroup[2] == 0){
    printf("41-60 0 0\n");
  }else{
    printf("41-60 %d %.2lf\n", vaccinated[2], ((double)vaccinated[2]/(double)c->popByAgeGroup[2])*100);
  }
  if(c->popByAgeGroup[3] == 0){
    printf("60+ 0 0\n");
  }else{
    printf("60+ %d %.2lf\n", vaccinated[3], ((double)vaccinated[3]/(double)c->popByAgeGroup[3])*100);
  }
}

void print_vaccination_ratio(Country *c, int vaccinated[]){
  printf("%s ", c->name);
  int overallPopulation = 0;
  int overallVaccinated = 0;
  for(int i = 0; i < 4; i++){
    overallVaccinated += vaccinated[i];
    overallPopulation += c->popByAgeGroup[i];
  }
  if(overallPopulation == 0 && overallVaccinated == 0){
    printf("0 0\n");
  }else{
    printf("%d %d\n", overallPopulation, overallVaccinated);
  }
}

void destroy_country(Country **c){
  free((*c)->name);
  (*c)->name = NULL;
  free(*c);
  *c = NULL;
}