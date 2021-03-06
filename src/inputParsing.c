#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/inputparsing.h"
#include "../include/hashmap.h"
#include "../include/country.h"
#include "../include/citizen.h"
#include "../include/virus.h"

void inputFileParsing(hashMap *countries, hashMap *citizens, hashMap *viruses, FILE *input, int bloomFilterSize){
  size_t line_size = 1024, bytes_read;
  char *line = malloc(line_size*sizeof(char)), *rest;
  char *id, *firstName, *lastName, *country_name, *age, *virus_name, *vacStatus, *date, *temp, *dupeVaccinationDate;
  int found, erroneousRecord;
  Country *country;
  Virus *virus;
  Citizen *citizen;
  while(!feof(input)){
    erroneousRecord = 0;
    bytes_read = getline(&line, &line_size, input);
    if(bytes_read == -1){
      break;
    }
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
    if(!erroneousRecord){
      country = (Country *) find_node(countries, country_name);
      if(country == NULL){
        country = create_country(country_name);
        insert(countries, country_name, country);        
      }
      citizen = (Citizen *) find_node(citizens, id);
      if(citizen == NULL){
        citizen = create_citizen(id, firstName, lastName, atoi(age), country);
        insert(citizens, id, citizen);        
      }
      virus = (Virus *) find_node(viruses, virus_name);
      if(virus == NULL){
        // Since we don't know exactly how many lines the input file will have
        // if we tell the skiplist we expect many fewer elements than those that will arrive,
        // the list will become quite 'dense'. 
        // However, as the citizens should be randomly distributed accross both vaccinated and non vaccinated
        // skiplists as well as over 12 viruses (24 skiplists in total), we don't necessarily have to
        // expect each skiplist to have too many elements; 100 is in the case of multiple records
        virus = create_virus(virus_name, 100, bloomFilterSize, 16);
        insert(viruses, virus_name, virus);        
      }
      if(strcmp(vacStatus, "NO") == 0){
        // Making sure there hasn't been a record read that lists the citizen
        // as vaccinated already.
        // If the bloom filter returns 0, there *definitely* hasn't been such a record.
        // If it returns 1, we make sure it isn't a false positive by searching for 
        // the node in the 'vaccinated for' the virus skiplist
        if(!lookup_in_virus_bloomFilter(virus, id)){
          insert_in_not_vaccinated_for_list(virus, atoi(id), citizen);
        }else if(!lookup_in_virus_vaccinated_for_list(virus, atoi(id))){
          insert_in_not_vaccinated_for_list(virus, atoi(id), citizen);
        }
      }else{
        if(lookup_in_virus_not_vaccinated_for_list(virus, atoi(id))){
          // Ignoring duplicate case where first record for same citizen and virus
          // mentions that the citizen is not vaccinated for the virus
          // and second record says citizen is vaccinated.
          continue;
        }
        if(lookup_in_virus_vaccinated_for_list(virus, atoi(id))){
          // Ignoring duplicate case where both records for same citizen and virus
          // mention that the citizen is vaccinated for the virus but the records
          // have different vaccination dates
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
  }
  free(line);
}
  
  
  
  
  