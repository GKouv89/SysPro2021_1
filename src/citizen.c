#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/citizen.h"

Citizen* create_citizen(const char *id, const char *firstName, const char *lastName, int age, Country *country){
  Citizen *c = malloc(sizeof(Citizen));
  c->id = malloc((strlen(id) + 1)*sizeof(char));
  strcpy(c->id, id);
  c->firstName = malloc((strlen(firstName) + 1)*sizeof(char));
  strcpy(c->firstName, firstName);
  c->lastName = malloc((strlen(lastName) + 1)*sizeof(char));
  strcpy(c->lastName, lastName);
  c->age = age;
  c->country = country;
  return c;
}

int isEqual_citizen(Citizen *c, unsigned char *str){
  if(strcmp(c->id, str) == 0){
    return 1;
  }else{
    return 0;
  }
}


void destroy_citizen(Citizen **cit){
  free((*cit)->id);
  (*cit)->id = NULL;
  free((*cit)->firstName);
  (*cit)->firstName = NULL;
  free((*cit)->lastName);
  (*cit)->lastName = NULL;
  (*cit)->country = NULL;
  free(*cit);
  *cit = NULL;
}