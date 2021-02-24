#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/skiplist.h"

void create_skiplist(skipList **s){
  (*s) = malloc(sizeof(skipList));
  (*s)->height = 1; 
  (*s)->levels = malloc(sizeof(*((*s)->levels)));
  create_list(&((*s)->levels[0]));
}

void insert_skipnode(skipList *s, int id){
  listNode **startingNodes = malloc(s->height*sizeof(listNode *));
  // we will have all the nodes where we went a level lower
  // therefore the boundaries for the insertion into that height's list
  // if the new node will reach that height
  int error = 0;
  search_skip(s, id, startingNodes, &error);
  if(error == 1){
    // ID already exists in skip list
    return;
  }
  int idHeight = 1;
  while(rand() % 2 == 1){
    idHeight++;
  }
  if(idHeight > s->height){
    printf("TALLER TALLER\n");
    list **temp = realloc(s->levels, idHeight*sizeof(*(s->levels)));
    assert(temp != NULL);
    s->levels = temp;
    listNode **temp2 = realloc(startingNodes, idHeight*sizeof(listNode *));
    assert(temp2 != NULL);
    startingNodes = temp2;
    for(int i = s->height; i < idHeight; i++){
      create_list(&(s->levels[i]));
      startingNodes[i] = NULL; // these new lists are empty
      // so no boundary exists for the insertion
    }
    s->height = idHeight;
  }
  listNode *prevConnection = NULL; // this will connect the node of a list
  // with the corresponding one in the list right above that
  listNode *currConnection;
  
  printf("BEFORE INSERTION, STARTINGNODES\n");
  for(int i = 0; i < s->height; i++){
    if(startingNodes[i] != NULL){
      printf("IN SEARCH_SKIP, startingNodes[%d]->id = %d\n", i, startingNodes[i]->id);
    }else{
      printf("IN SEARCH_SKIP, startingNodes is NULL\n");
    }
  }
  for(int i = 0; i < s->height ;i++){
    currConnection = insert_node(s->levels[i], startingNodes[i], id);
    printf("TEST: currConnection->id = %d\n", currConnection->id);
    currConnection->bottom = prevConnection;
    prevConnection = currConnection;
  }
}

void search_skip(skipList *s, int id, listNode *startingNodes[], int *error){
  boundaries *bounds_ret, *bounds_arg = malloc(sizeof(boundaries));
  bounds_arg->start = s->levels[s->height - 1]->front;
  bounds_arg->end = s->levels[s->height - 1]->rear;
  for(int i = s->height - 1; i >= 0; i--){
    printf("HEIGHT: %d\n", i);
    bounds_ret = search(s->levels[i], id, bounds_arg->start, bounds_arg->end, error);
    if(*error == 1){ // Element already in skiplist
      startingNodes[i] = NULL;// WIP: what do we insert in startingNodes?
      return;
    }
    startingNodes[i] = bounds_arg->start; 
    if(startingNodes[i] != NULL){
      printf("IN SEARCH_SKIP, startingNodes[%d]->id = %d\n", i, startingNodes[i]->id);
    }else{
      printf("IN SEARCH_SKIP, startingNodes is NULL\n");
    }
    // For insertion, for all lists except the bottom one,
    // the startingNode will be the node right after which the new one will be inserted
    // This is also the reason for which in insertion, we only require a starting node
    // We have modified the value of bounds_arg->start in the search function
    bounds_arg->start = bounds_ret->start;
    bounds_arg->end = bounds_ret->end;
  }
  free(bounds_arg);
}

void print_skiplist(skipList *s){
  for(int i = s->height - 1; i >= 0; i--){
    printf("LEVEL: %d\n", i);
    print_list(s->levels[i]);
  }
}

// void delete_skipnode(skipList *, int);


void destroy_skiplist(skipList **s){
  if(*s == NULL){
    return;
  }
  for(int i = 0; i < (*s)->height; i++){
    destroy_list(&((*s)->levels[i]));
  }
  free((*s)->levels);
  (*s)->levels = NULL;
  free(*s);
  (*s) = NULL;
}