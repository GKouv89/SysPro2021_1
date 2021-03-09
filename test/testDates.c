#include "acutest.h"

#include "../include/commands.h"

void test_date_comparison(){
  char *date1 = malloc(11*sizeof(char));
  strcpy(date1, "08-08-1999");
  char *date2 = malloc(11*sizeof(char));
  strcpy(date2, "30-08-1999");
  TEST_ASSERT(dateComparison(date1, date2) == 0);
  strcpy(date1, "08-08-1999");
  strcpy(date2, "09-09-1999");
  TEST_ASSERT(dateComparison(date1, date2) == 0);
  strcpy(date1, "08-08-1999");
  strcpy(date2, "09-09-2000");
  TEST_ASSERT(dateComparison(date1, date2) == 0);
  strcpy(date1, "08-08-1999");
  strcpy(date2, "13-07-1967");
  TEST_ASSERT(dateComparison(date1, date2) == -1);
  free(date1);
  free(date2);
}

TEST_LIST = {
  {"date_comparison", test_date_comparison},
  {NULL, NULL}
};