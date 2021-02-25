#include <time.h>
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

void test_skiplist_first_element_insertion(){
  time_t t;
  srand((unsigned) time(&t));
  skipList *s;
  create_skiplist(&s);
  int test_id = 45;
  printf("\n");
  insert_skipnode(s, test_id);
  print_skiplist(s);
  // also more interested on potential errors from valgrind
  destroy_skiplist(&s);
}

void test_skiplist_insertion(){
  time_t t;
  srand((unsigned) time(&t));
  skipList *s;
  create_skiplist(&s);
  int test_ids[] = {45, 679, 999, 3, 889, 102};
  printf("\n");
  for(int i = 0; i < 6; i++){
    #ifdef DEBUG
    printf("inserting %d\n", test_ids[i]);
    #endif
    insert_skipnode(s, test_ids[i]);
    #ifdef DEBUG
    print_skiplist(s);
    #endif
  }
  listNode *temp;
  for(int i = 0; i < s->height; i++){
    temp = s->levels[i]->front;
    while(temp->next){
      TEST_ASSERT(temp->id < temp->next->id);
      temp = temp->next;
    }
  }
  print_skiplist(s);
  destroy_skiplist(&s);
}

void test_insertion_new_data(){
  time_t t;
  srand((unsigned) time(&t));
  skipList *s;
  create_skiplist(&s);
  int test_ids[] = {507179, 27064, 780498, 136779, 371680, 537193, 193847/*, 169022, 194, 15451, 19729,
                    19904, 96398, 68318, 9094, 8200, 37871, 8990, 1618, 197, 21792, 1632*/};
  printf("\n");
  for(int i = 0; i < 7; i++){
    #ifdef DEBUG
    printf("inserting %d\n", test_ids[i]);
    #endif
    insert_skipnode(s, test_ids[i]);
    #ifdef DEBUG
    print_skiplist(s);
    #endif
  }
  listNode *temp;
  for(int i = 0; i < s->height; i++){
    temp = s->levels[i]->front;
    while(temp->next){
      TEST_ASSERT(temp->id < temp->next->id);
      temp = temp->next;
    }
  }
  print_skiplist(s);
  destroy_skiplist(&s);
}

TEST_LIST = {
  {"creation", test_skiplist_creation},
  {"destruction", test_skiplist_destruction},
  {"insertion", test_skiplist_insertion},
  {"insertion_first", test_skiplist_first_element_insertion},
  {"insertion_random", test_insertion_new_data},
  {NULL, NULL}
};