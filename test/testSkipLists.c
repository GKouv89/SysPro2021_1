#include "acutest.h"
#include "../include/skiplist.h"
#include "../include/linkedlist.h"

void test_skiplist_creation(){
  skipList *s;
  create_skiplist(&s);
  TEST_ASSERT(s->levels[0] != NULL);
  TEST_ASSERT(s->levels[0]->front == NULL);
  TEST_ASSERT(s->levels[0]->rear == NULL);
}

void test_skiplist_destruction(){
  skipList *s;
  create_skiplist(&s);
  destroy_skiplist(&s);
  TEST_ASSERT(s == NULL);
  // Here, we are more concerned with whether valgrind
  // will show any leaks
}

TEST_LIST = {
  {"creation", test_skiplist_creation},
  {"destruction", test_skiplist_destruction},
  {NULL, NULL}
};