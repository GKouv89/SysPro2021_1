#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/virus.h"

Virus* create_virus(const char *name, unsigned long long skiplist_max, int bloomfilter_max, int hf_count){
  Virus *v = malloc(sizeof(Virus));
  v->name = malloc((strlen(name) + 1)*sizeof(char));
  strcpy(v->name, name);
  create_skiplist(&(v->vaccinated_for), skiplist_max);
  create_skiplist(&(v->not_vaccinated_for), skiplist_max);
  create_bloomFilter(&(v->virusBF), bloomfilter_max, hf_count);
  return v;
}

void insert_in_virus_bloomFilter(Virus *v, unsigned char *str){
  insert_in_bloomFilter(v->virusBF, str);
}

char* insert_in_vaccinated_for_list(Virus *v, int citizenID, char *date, Citizen *citizen){
  return insert_skipnode(v->vaccinated_for, citizenID, date, citizen);
}

void insert_in_not_vaccinated_for_list(Virus *v, int citizenID, Citizen *citizen){
  insert_skipnode(v->not_vaccinated_for, citizenID, NULL, citizen);
}

void remove_from_not_vaccinated_for_list(Virus *v, int citizenID){
  delete_skipnode(v->not_vaccinated_for, citizenID);
}  

int lookup_in_virus_bloomFilter(Virus *v, unsigned char *str){
  return lookup_in_bloomFilter(v->virusBF, str);
}

listNode *lookup_in_virus_not_vaccinated_for_list(Virus *v, int citizenID){
  return lookup_skiplist(v->not_vaccinated_for, citizenID);
}

listNode *lookup_in_virus_vaccinated_for_list(Virus *v, int citizenID){
  return lookup_skiplist(v->vaccinated_for, citizenID);
}

void print_virus_skiplists(Virus *v){
  printf("Not vaccinated for %s:\n", v->name);
  print_skiplist(v->not_vaccinated_for);
  printf("Vaccinated for %s:\n", v->name);
  print_skiplist(v->vaccinated_for);
}

void print_non_vaccinated_for(Virus *v){
  print_skiplist_citizens(v->not_vaccinated_for);
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
