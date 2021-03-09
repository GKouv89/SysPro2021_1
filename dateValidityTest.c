#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(){
  char *date = malloc(12*sizeof(char));
  strcpy(date, "08-08-1999\n");
  if(strlen(date) != 11 && strlen(date) != 10){
    // invalid length
    // two different lengths to take into account
    // the second date that will have the 
    // end of line character attached to it as well
    free(date);
    return -1;
  }
  if(date[2] != '-' || date[5] != '-'){
    // both of these characters must be dashes, as they
    // separate day from month and month from year
    free(date);
    return -1;
  }
  char *rest;
  char *day = strtok_r(date, "-", &rest);
  char *month = strtok_r(NULL, "-", &rest);
  char *year = strtok_r(NULL, "", &rest);
  if(strstr(year, "\n") != NULL){
    year = strtok(year, "\n");
  }
  errno = 0;
  char *endstr;
  long val = strtol(day, &endstr, 10);
  if(val < 1 || val > 30 || *endstr != '\0'){
    free(date);
    return -1;
  }
  errno = 0;
  val = strtol(month, &endstr, 10);
  if(val < 1 || val > 12 || *endstr != '\0'){
    free(date);
    return -1;
  }
  errno = 0;
  val = strtol(year, &endstr, 10);
  if(*endstr != '\0'){
    free(date);
    return -1;
  }
  free(date);
  return 0;
}