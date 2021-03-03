#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/virus.h"

Virus* create_virus(const char *name, int skiplist_max, int bloomfilter_max, int hf_count){
  Virus *v = malloc(sizeof(Virus));
  v->name = malloc((strlen(name) + 1)*sizeof(char));
  strcpy(v->name, name);
  create_skiplist(&(v->vaccinated_for), skiplist_max);
  create_skiplist(&(v->not_vaccinated_for), skiplist_max);
  create_bloomFilter(&(v->virusBF), bloomfilter_max, hf_count);
  return v;
}

int lookup_in_virus_bloomFilter(Virus *v, unsigned char *str){
  lookup_in_bloomFilter(v->virusBF, str);
}

int isEqual_virus(Virus *v, unsigned char *str){
  if(strcmp(v->name, str) == 0){
    return 1;
  }else{
    return 0;
  }
}

void destroy_virus(Virus **vir){
  free((*vir)->name);
  (*vir)->name = NULL;
  destroy_bloomFilter(&((*vir)->virusBF));
  (*vir)->virusBF = NULL;
  destroy_skiplist(&((*vir)->vaccinated_for));
  (*vir)->vaccinated_for = NULL;
  destroy_skiplist(&((*vir)->not_vaccinated_for));
  (*vir)->not_vaccinated_for = NULL;
  free(*vir);
  *vir = NULL;
}
