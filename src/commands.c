#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../include/commands.h"
#include "../include/hashmap.h"
#include "../include/virus.h"
#include "../include/citizen.h"
#include "../include/country.h"

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

void vaccinateNow(Virus *v, unsigned char *ID, unsigned char *todays_date, Citizen *citizen){
  listNode *temp = lookup_in_virus_vaccinated_for_list(v, atoi(ID));
  if(temp != NULL){
    printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s.\n", ID, temp->vaccinationDate);
  }else{
    insert_in_virus_bloomFilter(v, ID);
    remove_from_not_vaccinated_for_list(v, atoi(ID));
    insert_in_vaccinated_for_list(v, atoi(ID), todays_date, citizen);
  }
}

void insertCitizenRecord(hashMap *viruses, hashMap *countries, hashMap *citizens, int bloomFiltersSize, char *rest){
  char *citizenID = strtok_r(NULL, " ", &rest);
  char *firstName = strtok_r(NULL, " ", &rest);
  char *lastName = strtok_r(NULL, " ", &rest);
  char *country = strtok_r(NULL, " ", &rest);
  char *age = strtok_r(NULL, " ", &rest);
  char *virusName = strtok_r(NULL, " ", &rest);
  Country *temp_country = (Country *) find_node(countries, country);
  if(temp_country == NULL){
    temp_country = create_country(country);
    insert(countries, country, temp_country);        
  }
  Citizen *temp_citizen = (Citizen *) find_node(citizens, citizenID);
  if(temp_citizen == NULL){
    temp_citizen = create_citizen(citizenID, firstName, lastName, atoi(age), temp_country);
    insert(citizens, citizenID, temp_citizen);        
  }
  Virus *temp_virus = (Virus *) find_node(viruses, virusName);
  if(temp_virus == NULL){
    temp_virus = create_virus(virusName, 7850000000, bloomFiltersSize, 16);
    insert(viruses, virusName, temp_virus);        
  }
  char *vacStatus = strtok_r(NULL, " ", &rest);
  char *vacDate = strtok_r(NULL, " ", &rest);
  if(strstr(vacStatus, "NO")){
    if(vacDate != NULL){
      printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n", citizenID, firstName, lastName, country, age, virusName, vacStatus, vacDate);
    }else{
      if(!lookup_in_virus_bloomFilter(temp_virus, citizenID)){
        insert_in_not_vaccinated_for_list(temp_virus, atoi(citizenID), temp_citizen);
      }else if(!lookup_in_virus_vaccinated_for_list(temp_virus, atoi(citizenID))){
        insert_in_not_vaccinated_for_list(temp_virus, atoi(citizenID), temp_citizen);
      }
    }
  }else{
    char *dateToken = strtok(vacDate, "\n");
    listNode *temp = lookup_in_virus_not_vaccinated_for_list(temp_virus, atoi(citizenID));
    if(temp != NULL){
      remove_from_not_vaccinated_for_list(temp_virus, atoi(citizenID));
      insert_in_virus_bloomFilter(temp_virus, citizenID);
      insert_in_vaccinated_for_list(temp_virus, atoi(citizenID), dateToken, temp_citizen);
      return;
    }  
    temp = lookup_in_virus_vaccinated_for_list(temp_virus, atoi(citizenID));
    if(temp != NULL){
      printf("ERROR: CITIZEN ALREADY VACCINATED ON %s\n", temp->vaccinationDate);
      return;
    }
    insert_in_virus_bloomFilter(temp_virus, citizenID);
    insert_in_vaccinated_for_list(temp_virus, atoi(citizenID), dateToken, temp_citizen);
  }
}

void list_non_vaccinated_persons(hashMap *viruses, unsigned char *virusName){
  Virus *v = (Virus *) find_node(viruses, virusName);
  if(v == NULL){
    printf("No info available on virus %s\n", virusName);
    return;
  }
  print_non_vaccinated_for(v);
}

void popStatusByAge(hashMap *viruses, hashMap *countries, unsigned char *country_name, unsigned char *virusName, char *startDate, char *endDate){
  Virus *v = (Virus *) find_node(viruses, virusName);
  if(v == NULL){
    printf("ERROR: NO SUCH VIRUS EXISTS\n");
    return;
  }
  if(country_name == NULL){
    lookup_popStatus_all(countries, 1, v, startDate, endDate);
  }else{
    Country *c = (Country *) find_node(countries, country_name);
    if(c == NULL){
      printf("ERROR: NO SUCH COUNTRY\n");
      return;
    }
    population *pop = skiplist_vac_status_country(v->vaccinated_for, 1, c, startDate, endDate);
    population *popNo = skiplist_vac_status_country(v->not_vaccinated_for, 0, c, startDate, endDate);
    printf("%s ", c->name);
    int overallPop[4] = {0, 0, 0, 0};
    for(int i = 0; i < 4; i++){
      overallPop[i] += pop->popByAgeGroup[i] + popNo->popByAgeGroup[i];
    }
    if(overallPop[0] == 0){
      printf("0-20 0 0%%\n");
    }else{
      printf("0-20 %d %.2lf%%\n", pop->popByAgeGroup[0], ((double)pop->popByAgeGroup[0]/(double)overallPop[0])*100);
    }
    if(overallPop[1] == 0){
      printf("21-40 0 0%%\n");
    }else{
      printf("21-40 %d %.2lf%%\n", pop->popByAgeGroup[1], ((double)pop->popByAgeGroup[1]/(double)overallPop[1])*100);
    }
    if(overallPop[2] == 0){
      printf("41-60 0 0%%\n");
    }else{
      printf("41-60 %d %.2lf%%\n", pop->popByAgeGroup[2], ((double)pop->popByAgeGroup[2]/(double)overallPop[2])*100);
    }
    if(overallPop[3] == 0){
      printf("60+ 0 0%%\n");
    }else{
      printf("60+ %d %.2lf%%\n", pop->popByAgeGroup[3], ((double)pop->popByAgeGroup[3]/(double)overallPop[3])*100);
    }
    free(pop);
    free(popNo);
  }
}

void popStatus(hashMap *viruses, hashMap *countries, unsigned char *country_name, unsigned char *virusName, char *startDate, char *endDate){
  Virus *v = (Virus *) find_node(viruses, virusName);
  if(v == NULL){
    printf("ERROR: NO SUCH VIRUS EXISTS\n");
    return;
  }
  if(country_name == NULL){
    lookup_popStatus_all(countries, 0, v, startDate, endDate);
  }else{
    Country *c = (Country *) find_node(countries, country_name);
    if(c == NULL){
      printf("ERROR: NO SUCH COUNTRY\n");
      return;
    }
    population *pop = skiplist_vac_status_country(v->vaccinated_for, 1, c, startDate, endDate);
    population *popNo = skiplist_vac_status_country(v->not_vaccinated_for, 0, c, startDate, endDate);
    int overallVac = 0;
    int overallPop = 0;
    for(int i = 0; i < 4; i++){
      overallVac += pop->popByAgeGroup[i];
      overallPop += pop->popByAgeGroup[i] + popNo->popByAgeGroup[i];
    }
    free(pop);
    free(popNo);
    printf("%s ", c->name);
    if(overallPop == 0 && overallVac == 0){
      printf("0 0%%\n");
    }else{
      printf("%d %.2lf%%\n", overallVac, ((double)overallVac/(double)overallPop)*100);
    }
  }
}
