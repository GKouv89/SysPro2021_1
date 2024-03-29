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

void print_vaccination_ratios_byAge(Country *c, struct vaccinationsAgeGroup *vacced, struct vaccinationsAgeGroup *notVacced){
  printf("%s\n", c->name);
  int overallPopByGroup[4];
  for(int i = 0; i < 4; i++){
    overallPopByGroup[i] = vacced->overall[i] + notVacced->overall[i];
  }
  if(overallPopByGroup[0] == 0){
    printf("0-20 0 0%%\n");
  }else{
    printf("0-20 %d %.2lf%%\n", vacced->inRange[0], ((double)vacced->inRange[0]/(double)overallPopByGroup[0])*100);
  }
  if(overallPopByGroup[1] == 0){
    printf("21-40 0 0%%\n");
  }else{
    printf("21-40 %d %.2lf%%\n", vacced->inRange[1], ((double)vacced->inRange[1]/(double)overallPopByGroup[1])*100);
  }
  if(overallPopByGroup[2] == 0){
    printf("41-60 0 0%%\n");
  }else{
    printf("41-60 %d %.2lf%%\n", vacced->inRange[2], ((double)vacced->inRange[2]/(double)overallPopByGroup[2])*100);
  }
  if(overallPopByGroup[3] == 0){
    printf("60+ 0 0%%\n");
  }else{
    printf("60+ %d %.2lf%%\n", vacced->inRange[3], ((double)vacced->inRange[3]/(double)overallPopByGroup[3])*100);
  }
  free(vacced);
  free(notVacced);
}

void print_vaccination_ratio(Country *c, struct vaccinations *vacced, struct vaccinations *notVacced){
  printf("%s ", c->name);
  int overallPop = vacced->overall + notVacced->overall;
  if(overallPop == 0){
    printf("0 0%%\n");
  }else{
    printf("%d %.2lf%%\n", vacced->inRange, ((double)vacced->inRange/(double)overallPop)*100);
  }
  free(vacced);
  free(notVacced);
}

void destroy_country(Country **c){
  free((*c)->name);
  (*c)->name = NULL;
  free(*c);
  *c = NULL;
}