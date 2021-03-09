#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <errno.h>

#include "../include/inputparsing.h"
#include "../include/hashmap.h"
#include "../include/virus.h"
#include "../include/country.h"
#include "../include/citizen.h"
#include "../include/commands.h"

int dateValidity(char *);

int main(int argc, char *argv[]){
  srand(time(NULL));
  if(argc != 5){
    printf("Flags:\n");
    printf("-c file_name: records will be read from the file named file_name.\n");
    printf("-b bloomSize: the size of the bloom filters in *kilobytes*\n");
    printf("It is necessary you use them both and provide an argument for each one,\n");
    printf("but the order in which you use them doesn't matter.\n");
    exit(1);
  }
  if(strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-b") != 0){
    printf("invalid option -- '%s'\n", argv[1]);
    printf("Type ./vaccineMonitor to see proper usage\n");
    exit(2);    
  }else if(strcmp(argv[3], "-b") != 0 && strcmp(argv[3], "-c") != 0){
    printf("invalid option -- '%s'\n", argv[3]);
    printf("Type ./vaccineMonitor to see proper usage\n");
    exit(2);    
  }
  
  char *fileName = malloc(255*sizeof(char));
  int bloomFiltersSize;
  if(strcmp(argv[1], "-c") == 0){
    strcpy(fileName, argv[2]);
    bloomFiltersSize = atoi(argv[4]);
  }else{
    strcpy(fileName, argv[4]);
    bloomFiltersSize = atoi(argv[2]);
  }
  FILE *recordsFile = fopen(fileName, "r");
  assert(recordsFile != NULL);
  
  hashMap *country_map, *virus_map, *citizen_map;
  // Prime bucket of numbers for all hashmaps
  // virusesFile has only about 12 viruses
  // and countriesFile contains 195 countries
  // so the primes were chosen to be an order of magnitude
  // smaller than the size of the respective file
  // for citizens, perhaps the input file will have few lines,
  // but perhaps it will have upwards of 1000 records.
  // In any case, this allows for insertion of multiple records
  // after reading the input file
  create_map(&country_map, 43, Country_List);
  create_map(&virus_map, 3, Virus_List);
  create_map(&citizen_map, 101, Citizen_List);
  
  // Calculating current date, just in case it is necessary for vaccinateNow //
  time_t t = time(NULL);
  struct tm currDate;
  char *date = malloc(11*sizeof(char));
  char *day, *month, *year;
  day = malloc(3*sizeof(char));
  month = malloc(3*sizeof(char));
  year = malloc(5*sizeof(char));
  currDate = *localtime(&t);
  if(currDate.tm_mday < 10){
    sprintf(day, "0%d", currDate.tm_mday);
  }else{
    sprintf(day, "%d", currDate.tm_mday);
  }
  if(currDate.tm_mon + 1 < 10){
    sprintf(month, "0%d", currDate.tm_mon + 1);
  }else{
    sprintf(month, "%d", currDate.tm_mon + 1);
  }
  sprintf(year, "%d", currDate.tm_year + 1900);
  sprintf(date, "%s-%s-%s", day, month, year);
  ////////////////////////////////////////////////////////////////////////////////
  
  inputFileParsing(country_map, citizen_map, virus_map, recordsFile, bloomFiltersSize);
  /// MAIN MENU ///
  size_t command_length = 1024, chars_read;
  char *command = malloc(command_length*sizeof(char)), *rest;
  char *comm_name, *citizenID, *virusName, *firstName, *lastName, *country, *vacStatus, *vacDate, *token;
  int age;
  citizenID = malloc(5*sizeof(char));
  virusName = malloc(13*sizeof(char));
  firstName = malloc(13*sizeof(char));
  lastName = malloc(13*sizeof(char));
  country = malloc(30*sizeof(char));
  vacStatus = malloc(4*sizeof(char));
  vacDate = malloc(11*sizeof(char));
  char *date1, *date2;
  date1 = malloc(11*sizeof(char));
  date2 = malloc(11*sizeof(char));
  Country *temp_country;
  Citizen *temp_citizen;
  Virus *temp_virus;
  while(1){
    chars_read = getline(&command, &command_length, stdin);
    comm_name = strtok_r(command, " ", &rest);
    if(strcmp(comm_name, "/vaccineStatusBloom") == 0){
      if(2 != sscanf(rest, "%s %s", citizenID, virusName)){
        printf("Bad arguments to /vaccineStatusBloom. Try again.\n");
      }else{
        vaccineStatusBloom(virus_map, citizenID, virusName);
      }
    }else if(strcmp(comm_name, "/vaccineStatus") == 0){
      if(2 == sscanf(rest, "%s %s", citizenID, virusName)){
        vaccineStatus(virus_map, citizenID, virusName);
      }else if(1 == sscanf(rest, "%s", citizenID)){
        vaccineStatusAll(virus_map, citizenID);
      }else{
        printf("Bad arguments to /vaccineStatus. Try again.\n");
      }
    }else if(strcmp(comm_name, "/vaccinateNow") == 0){
      if(sscanf(rest, "%s %s %s %s %d %s", citizenID, firstName, lastName, country, &age, virusName) == 6){
        // Before calling vaccinateNow, we must first see if the citizen, country and virus exist in
        // the hashtables
        temp_country = (Country *) find_node(country_map, country);
        if(temp_country == NULL){
          temp_country = create_country(country);
          insert(country_map, country, temp_country);        
        }
        temp_citizen = (Citizen *) find_node(citizen_map, citizenID);
        if(temp_citizen == NULL){
          temp_citizen = create_citizen(citizenID, firstName, lastName, age, temp_country);
          insert(citizen_map, citizenID, temp_citizen);        
        }
        temp_virus = (Virus *) find_node(virus_map, virusName);
        if(temp_virus == NULL){
          temp_virus = create_virus(virusName, 100, bloomFiltersSize, 16);
          insert(virus_map, virusName, temp_virus);        
        }
        vaccinateNow(temp_virus, citizenID, date, temp_citizen);
      }else{
        printf("Bad arguments to /vaccinateNow. Try again.\n");
      }
    }else if(strcmp(comm_name, "/insertCitizenRecord") == 0){
      insertCitizenRecord(virus_map, country_map, citizen_map, bloomFiltersSize, rest);
    }else if(strcmp(comm_name, "/list-nonVaccinated-Persons") == 0){
      if(sscanf(rest, "%s", virusName) == 1){
        list_non_vaccinated_persons(virus_map, virusName);
      }else{
        printf("Bad arguments to /list-nonVaccinated-Persons. Try again.\n");
      }
    }else if(strcmp(comm_name, "/popStatusByAge") == 0){
      // First, checking for argument count.
      if(sscanf(rest, "%s %s %s %s", country, virusName, date1, date2) == 4){
        // checking for date validity
      }else if(sscanf(rest, "%s %s %s", virusName, date1, date2) == 3){
        // checking for date validity
      }else{
        printf("Bad arguments to /popStatusByAge. Try again.\n");
      }
    }else if(strcmp(comm_name, "/exit\n") == 0){
      break;
    }else{
      printf("Unknown command %s. Try again.\n", comm_name);
    }
  }
  
  
  ////////////////////////////////////////////////////////
  // Here, the assertion will take place through prints //
  ////////////////////////////////////////////////////////
  // for(int i = 0; i < country_map->noOfBuckets; i++){
    // bucketNode *temp = country_map->map[i]->bl->front;
    // while(temp){
      // printf("%s has a population of %d in the 0-20 age group.\n", ((Country *)temp->content)->name, ((Country *)temp->content)->popByAgeGroup[0]);
      // printf("%s has a population of %d in the 21-40 age group.\n", ((Country *)temp->content)->name, ((Country *)temp->content)->popByAgeGroup[1]);
      // printf("%s has a population of %d in the 41-60 age group.\n", ((Country *)temp->content)->name, ((Country *)temp->content)->popByAgeGroup[2]);
      // printf("%s has a population of %d in the 60+ age group.\n", ((Country *)temp->content)->name, ((Country *)temp->content)->popByAgeGroup[3]);
      // temp = temp->next;
    // }
  // }
  
  destroy_map(&citizen_map);
  destroy_map(&virus_map);
  destroy_map(&country_map);
  assert(fclose(recordsFile) == 0);
  free(fileName);
  free(command);
  free(citizenID);
  free(virusName);
  free(firstName);
  free(lastName);
  free(country);
  free(vacStatus);
  free(vacDate);
  free(date);
  free(day);
  free(month);
  free(year);
  return 0;
}

int dateValidity(char *date){
  if(strlen(date) != 10){
    // invalid length
    return -1;
  }
  if(date[2] != '-' || date[5] != '-'){
    // both of these characters must be dashes, as they
    // separate day from month and month from year
    return -1;
  }
  char *rest;
  char *day = strtok_r(date, "-", &rest);
  char *month = strtok_r(NULL, "-", &rest);
  char *year = strtok_r(NULL, "", &rest);
  // Making sure that no end of line character is at the end of the string
  // which should happen for the second date
}