#include <time.h>
#include "acutest.h"
#include "../include/hashmap.h"
#include "../include/citizen.h"
#include "../include/country.h"
#include "../include/virus.h"

char* ids[5] = {"179", "18", "2065", "345", "345"};

char* firstNames[5] = {"BRIAN", "EMILIA", "ANNA", "ERIC", "ERIC"};

char* lastNames[5] = {"DUCOUTEAU", "DEBRIES", "SUN", "BASILICH", "BASILICH"};

char *ages[5] = {"27", "31", "8", "56", "56"};

char *countries[5] = {"CANADA", "NETHERLANDS", "UNITED-STATES-OF-AMERICA", "GERMANY", "GERMANY"};

char *viruses[5] = {"COVID-19", "POLIO", "INFLUENZA-A", "HEP-A", "HEP-A"};

char *vacStatuses[5] = {"NO", "YES", "NO", "YES", "YES"};

char *dates[5] = {"", "08-08-2020", "17-03-2020", "18-11-2020", "18-10-2020"};

void test_file_parsing(){
  FILE *fp = fopen("files/testInputFile.txt", "r");
  TEST_ASSERT(fp != NULL);
  size_t line_size = 1024;
  char *line = malloc(line_size*sizeof(char)), *rest;
  char *id, *firstName, *lastName, *country, *age, *virus, *vacStatus, *date, *temp;
  for(int i = 0; i < 5; i++){
    getline(&line, &line_size, fp);
    id = strtok_r(line, " ", &rest);
    firstName = strtok_r(NULL, " ", &rest);
    lastName = strtok_r(NULL, " ", &rest);
    country = strtok_r(NULL, " ", &rest);
    age = strtok_r(NULL, " ", &rest);
    virus = strtok_r(NULL, " ", &rest);
    vacStatus = strtok_r(NULL, " ", &rest);
    date = strtok_r(NULL, " ", &rest);
    TEST_ASSERT(strcmp(id, ids[i]) == 0);
    TEST_ASSERT(strcmp(firstName, firstNames[i]) == 0);
    TEST_ASSERT(strcmp(lastName, lastNames[i]) == 0);
    TEST_ASSERT(strcmp(age, ages[i]) == 0);
    TEST_ASSERT(strcmp(country, countries[i]) == 0);
    TEST_ASSERT(strcmp(age, ages[i]) == 0);
    TEST_ASSERT(strcmp(virus, viruses[i]) == 0);
    if(strstr(vacStatus, "NO")){
      // normally, we would expect no date after that
      if(date != NULL){
        printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n", id, firstName, lastName, country, age, virus, vacStatus, date);
      }else{
        temp = strtok(vacStatus, "\n");
        TEST_ASSERT(strcmp(temp, vacStatuses[i]) == 0);
      }
    }else{
      TEST_ASSERT(strcmp(vacStatus, vacStatuses[i]) == 0);
      temp = strtok(date, "\n");
      TEST_ASSERT(strcmp(temp, dates[i]) == 0);
    }
  }
  TEST_ASSERT(fclose(fp) == 0);
  free(line);
}

void test_dupe_rejection(){
  FILE *fp = fopen("files/testInputFile.txt", "r");
  TEST_ASSERT(fp != NULL);
  size_t line_size = 1024;
  char *line = malloc(line_size*sizeof(char)), *rest;
  char *id, *firstName, *lastName, *country_name, *age, *virus_name, *vacStatus, *date, *temp, *dupeVaccinationDate;
  hashMap *country_map, *virus_map, *citizen_map;
  create_map(&country_map, 2, Country_List);
  create_map(&virus_map, 2, Virus_List);
  create_map(&citizen_map, 2, Citizen_List);
  int found, erroneousRecord;
  Country *country;
  Virus *virus;
  Citizen *citizen;
  for(int i = 0; i < 5; i++){
    erroneousRecord = 0;
    getline(&line, &line_size, fp);
    id = strtok_r(line, " ", &rest);
    firstName = strtok_r(NULL, " ", &rest);
    lastName = strtok_r(NULL, " ", &rest);
    country_name = strtok_r(NULL, " ", &rest);
    age = strtok_r(NULL, " ", &rest);
    virus_name = strtok_r(NULL, " ", &rest);
    vacStatus = strtok_r(NULL, " ", &rest);
    date = strtok_r(NULL, " ", &rest);
    if(strstr(vacStatus, "NO")){
      // normally, we would expect no date after that
      if(date != NULL){
        erroneousRecord = 1;
        printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n", id, firstName, lastName, country_name, age, virus_name, vacStatus, date);
      }else{
        temp = strtok(vacStatus, "\n");
      }
    }else{
      temp = strtok(date, "\n");
    }
    //////////NEW//////////
    if(!erroneousRecord){
      country = (Country *) find_node(country_map, country_name);
      if(country == NULL){
        country = create_country(country_name);
        insert(country_map, country_name, country);        
      }
      citizen = (Citizen *) find_node(citizen_map, id);
      if(citizen == NULL){
        citizen = create_citizen(id, firstName, lastName, atoi(age), country);
        insert(citizen_map, id, citizen);        
      }
      virus = (Virus *) find_node(virus_map, virus_name);
      if(virus == NULL){
        virus = create_virus(virus_name, 2, 2, 2);
        insert(virus_map, virus_name, virus);        
      }
      if(strcmp(vacStatus, "NO") == 0){
        if(!lookup_in_virus_bloomFilter(virus, id)){
          insert_in_not_vaccinated_for_list(virus, atoi(id), citizen);
        }else if(!lookup_in_virus_vaccinated_for_list(virus, id)){
          insert_in_not_vaccinated_for_list(virus, atoi(id), citizen);
        }
      }else{
        if(lookup_in_virus_vaccinated_for_list(virus, id)){
          continue;
        }
        insert_in_virus_bloomFilter(virus, id);
        dupeVaccinationDate = insert_in_vaccinated_for_list(virus, atoi(id), date, citizen);
        if(dupeVaccinationDate != NULL){
          printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n", id, firstName, lastName, country_name, age, virus_name, vacStatus, date);
          printf("CITIZEN ALREADY VACCINATED ON %s\n", dupeVaccinationDate);
        }
      }
    }
    ///////////////////////
    
  }
  TEST_ASSERT(fclose(fp) == 0);
  free(line);
  destroy_map(&citizen_map);
  destroy_map(&virus_map);
  destroy_map(&country_map);
}

TEST_LIST = {
  {"file_parsing", test_file_parsing},
  {"duplicate_rejection", test_dupe_rejection},
  {NULL, NULL}
};