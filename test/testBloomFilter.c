#include "acutest.h"
#include "../include/bloomfilter.h"

void test_shift_amount(){
  unsigned char bloom[2] = {0, 0};
  unsigned char mask;
  int mod_results[] = {3, 7, 11};
  int div, mod;
  for(int i = 0; i < 3; i++){
    div = mod_results[i]/8; // which byte of the array are we in?
    mod = mod_results[i]%8; // which bit of the byte do we wish to change?
    mask = 1;
    mask = mask << (8 - mod - 1);
    bloom[div] = bloom[div] | mask;
  }
  TEST_CHECK(bloom[0] == 17);
  TEST_MSG("Expected: 17\tActual: %u\n", bloom[0]);
  TEST_CHECK(bloom[1] == 16);
  TEST_MSG("Expected: 16\tActual: %u\n", bloom[1]);
}

void test_insertion_through_lookup(){
  // This code printed the info for our next test
  // In specific, the value of mod_results[i] for each value of i
  // in hash_i if we set K as 2
  char **res = malloc(3*sizeof(char *));
  res[0] = malloc(2*sizeof(char));
  strcpy(res[0], "3");
  res[1] = malloc(2*sizeof(char));
  strcpy(res[1], "7");
  res[2] = malloc(3*sizeof(char));
  strcpy(res[2], "11");
  // for(int i = 0; i < 3; i++){
    // for(int j = 0; j < 2; j++){
      // printf("Value of hash_%d for %d is %lu\n", j, i, hash_i(res[i], j));
    // }
  // }
  // For the purposes of this experiment, size is 2 bytes
  bloomFilter *bf;
  create_bloomFilter(&bf, 2, 2);
  for(int i = 0; i < 3; i++){
    insert_in_bloomFilter(bf, res[i]);
  }
  TEST_ASSERT(bf->filter[0] == 9);
  TEST_ASSERT(bf->filter[1] == 136);
  for(int i = 0; i < 3; i++){
    TEST_ASSERT(lookup_in_bloomFilter(bf, res[i]) == 1);
    // AS LONG AS ELEMENTS HAVE BEEN INSERTED IN THE FILTER
    // THE FILTER IN NO WAY RETURNS 0
    free(res[i]);
    res[i] = NULL;
  }
  free(res);
  res = NULL;
  destroy_bloomFilter(&bf);
}

TEST_LIST = {
  {"shift_amount", test_shift_amount},
  {"insertion_and_lookup", test_insertion_through_lookup},
  {NULL, NULL}
};