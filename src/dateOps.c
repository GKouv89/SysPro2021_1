#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../include/dateOps.h"

void dateValidity(char *date1, char *date2){
  if(!dateFormatValidity(date1) && !dateFormatValidity(date2)){
    if(dateComparison(date1, date2)){
      printf("End date before start date.\n");
    }
  }else{
    printf("ERROR.\n");
  }
}

///////////////////////////////////////////////////////
// This function asserts that the dates given        //
// in popStatus and popStatusByAge are in the format //
// DD-MM-YYYY.                                       //
///////////////////////////////////////////////////////

int dateFormatValidity(char *date){
  char *tempdate = malloc(11*sizeof(char));
  strcpy(tempdate, date);
  if(strlen(tempdate) != 11 && strlen(tempdate) != 10){
    // invalid length
    // two different lengths to take into account
    // the second date that will have the 
    // end of line character attached to it as well
    return -1;
  }
  if(tempdate[2] != '-' || tempdate[5] != '-'){
    // both of these characters must be dashes, as they
    // separate day from month and month from year
    return -1;
  }
  char *rest;
  char *day = strtok_r(tempdate, "-", &rest);
  char *month = strtok_r(NULL, "-", &rest);
  char *year = strtok_r(NULL, "", &rest);
  // Making sure that no end of line character is at the end of the string
  // which should happen for the second date
  if(strstr(year, "\n") != NULL){
    year = strtok(year, "\n");
  }
  errno = 0;
  char *endstr;
  long val = strtol(day, &endstr, 10);
  if(val < 1 || val > 30 || *endstr != '\0'){
    free(tempdate);
    return -1;
  }
  errno = 0;
  val = strtol(month, &endstr, 10);
  if(val < 1 || val > 12 || *endstr != '\0'){
    free(tempdate);
    return -1;
  }
  errno = 0;
  val = strtol(year, &endstr, 10);
  if(*endstr != '\0'){
    free(tempdate);
    return -1;
  }
  free(tempdate);
  return 0;
}

/////////////////////////////////////////////////////////
// This function makes sure that the date arguments    //
// given in popStatus and popStatusByAge are           //
// in correct chronological order, i.e. the first date //
// is not later than the second one.                   //
/////////////////////////////////////////////////////////
int dateComparison(char *date1, char *date2){
  char *tempdate1 = malloc(11*sizeof(char));
  char *tempdate2 = malloc(12*sizeof(char));
  strcpy(tempdate1, date1);
  strcpy(tempdate2, date2);
  char *rest1, *rest2;
  char *day1 = strtok_r(tempdate1, "-", &rest1);
  char *month1 = strtok_r(NULL, "-", &rest1);
  char *year1 = strtok_r(NULL, "-", &rest1);
  char *day2 =  strtok_r(tempdate2, "-", &rest2);
  char *month2 = strtok_r(NULL, "-", &rest2);
  char *year2 = strtok_r(NULL, "-", &rest2);
  char *endptr;
  long y2, y1, m2, m1, d2, d1;
  y2 = strtol(year2, &endptr, 10);
  y1 = strtol(year1, &endptr, 10);
  m2 = strtol(month2, &endptr, 10);
  m1 = strtol(month1, &endptr, 10);
  d2 = strtol(day2, &endptr, 10);
  d1 = strtol(day1, &endptr, 10);
  if(y2 < y1){
    free(tempdate1);
    free(tempdate2);
    return -1;
  }else if(y2 == y1 && m2 < m1){
    free(tempdate1);
    free(tempdate2);
    return -1;
  }else if(y2 == y1 && m2 == m1 && d2 < d1){
    free(tempdate1);
    free(tempdate2);
    return -1;
  }
  free(tempdate1);
  free(tempdate2);
  return 0;
}
