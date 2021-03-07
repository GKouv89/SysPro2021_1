#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/inputparsing.h"
#include "../include/hashmap.h"
#include "../include/virus.h"
#include "../include/country.h"
#include "../include/citizen.h"
#include "../include/commands.h"

int main(int argc, char *argv[]){
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
  
  inputFileParsing(country_map, citizen_map, virus_map, recordsFile, bloomFiltersSize);
  /// MAIN MENU ///
  size_t command_length = 1024, chars_read;
  char *command = malloc(command_length*sizeof(char)), *rest;
  char *comm_name, *citizenID, *virusName;
  citizenID = malloc(5*sizeof(char));
  virusName = malloc(13*sizeof(char));
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
    }else if(strcmp(comm_name, "/exit\n") == 0){
      break;
    }else{
      printf("Unknown command %s. Try again.\n", comm_name);
    }
  }
  
  
  ////////////////////////////////////////////////////////
  // Here, the assertion will take place through prints //
  ////////////////////////////////////////////////////////
  // for(int i = 0; i < virus_map->noOfBuckets; i++){
    // bucketNode *temp = virus_map->map[i]->bl->front;
    // while(temp){
      // print_virus_skiplists((Virus *) temp->content);
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
  return 0;
}