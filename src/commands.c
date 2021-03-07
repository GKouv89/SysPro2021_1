#include <stdio.h>
#include <stdlib.h>

#include "../include/commands.h"
#include "../include/hashmap.h"
#include "../include/virus.h"

void vaccineStatusBloom(hashMap *viruses, unsigned char *citizenID, unsigned char *virusName){
  Virus *v = (Virus *) find_node(viruses, virusName);
  if(!lookup_in_virus_bloomFilter(v, citizenID)){
    printf("NOT VACCINATED\n");
  }else{
    printf("MAYBE\n");
  }
}

void vaccineStatus(hashMap *viruses, unsigned char *citizenID, unsigned char *virusName){
  Virus *v = (Virus *) find_node(viruses, virusName);
  listNode *temp = lookup_in_virus_vaccinated_for_list(v, atoi(citizenID));
  if(temp == NULL){
    printf("NOT VACCINATED\n");
  }else{
    printf("VACCINATED ON %s\n", temp->vaccinationDate);
  }
}

void vaccineStatusAll(hashMap *viruses, unsigned char *citizenID){
  lookup_vacStatus_all(viruses, citizenID);
}