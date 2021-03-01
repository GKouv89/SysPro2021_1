#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/skiplist.h"

void create_skiplist(skipList **s, int exp_data_count){
  (*s) = malloc(sizeof(skipList));
  (*s)->height = 1; 
  // (*s)->max_height = log10(exp_data_count)/log10(2);
  (*s)->max_height = log2(exp_data_count);
  (*s)->levels = malloc(sizeof(*((*s)->levels)));
  create_list(&((*s)->levels[0]));
}

void insert_skipnode(skipList *s, int id, char *vacDate, Citizen *c){
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
  while(rand() % 1000 < 1000*p && idHeight < s->max_height){
    idHeight++;
  }
  if(idHeight > s->height){
    #ifdef DEBUG
    printf("TALLER TALLER\n");
    #endif
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
  
  #ifdef DEBUG
  printf("BEFORE INSERTION, STARTINGNODES\n");
  for(int i = 0; i < s->height; i++){
    if(startingNodes[i] != NULL){
      printf("IN SEARCH_SKIP, startingNodes[%d]->id = %d\n", i, startingNodes[i]->id);
    }else{
      printf("IN SEARCH_SKIP, startingNodes is NULL\n");
    }
  }
  #endif
  for(int i = 0; i < s->height ;i++){
    if(i == 0){
      currConnection = insert_node(s->levels[i], startingNodes[i], id, vacDate, c);
    }else{
      currConnection = insert_node(s->levels[i], startingNodes[i], id, NULL, NULL);
    }
    #ifdef DEBUG
    printf("TEST: currConnection->id = %d\n", currConnection->id);
    #endif
    currConnection->bottom = prevConnection;
    prevConnection = currConnection;
    startingNodes[i] = NULL;
  }
  free(startingNodes);
}

void search_skip(skipList *s, int id, listNode *startingNodes[], int *error){
  boundaries *bounds_ret = malloc(sizeof(boundaries));
  boundaries *bounds_arg = malloc(sizeof(boundaries));
  bounds_arg->start = s->levels[s->height - 1]->front;
  bounds_arg->end = s->levels[s->height - 1]->rear;
  listNode *futureSN;
  for(int i = s->height - 1; i >= 0; i--){
    #ifdef DEBUG
    printf("HEIGHT: %d\n", i);
    #endif
    search(s->levels[i], id, bounds_arg->start, bounds_arg->end, &bounds_ret, error, &futureSN);
    if(*error == 1){ // Element already in skiplist
      startingNodes[i] = NULL;// WIP: what do we insert in startingNodes?
      return;
    }
    startingNodes[i] = futureSN;
    #ifdef DEBUG
    if(startingNodes[i] != NULL){
      printf("IN SEARCH_SKIP, startingNodes[%d]->id = %d\n", i, startingNodes[i]->id);
    }else{
      printf("IN SEARCH_SKIP, startingNodes[%d] is NULL\n", i);
    }
    #endif
    // For insertion, for all lists except the bottom one,
    // the startingNode will be the node right after which the new one will be inserted
    // This is also the reason for which in insertion, we only require a starting node
    // We have modified the value of bounds_arg->start in the search function
    bounds_arg->start = bounds_ret->start;
    bounds_arg->end = bounds_ret->end;
  }
  free(bounds_arg);
  free(bounds_ret);
}

///////////////////////////////////////////////////////////////////////////////////////
// Search is used to locate where a node will be placed after insertion in a list    //
// Lookup on the other hand is used to locate whether a node is in the skiplist and, //
// if so, info on that citizen's vaccination state                                   //
///////////////////////////////////////////////////////////////////////////////////////

void* lookup_skiplist(skipList *s, int id, int lookupMode){
  boundaries *bounds_ret = malloc(sizeof(boundaries));
  boundaries *bounds_arg = malloc(sizeof(boundaries));
  bounds_arg->start = s->levels[s->height - 1]->front;
  bounds_arg->end = s->levels[s->height - 1]->rear;
  listNode *futureSN;
  int *found;
  *found = 0;
  for(int i = s->height - 1; i >= 0; i--){
    search(s->levels[i], id, bounds_arg->start, bounds_arg->end, &bounds_ret, found, &futureSN);
    // futureSN is ignored here
    if(*found == 1){ // Element already in skiplist
      break;
    }
    bounds_arg->start = bounds_ret->start;
    bounds_arg->end = bounds_ret->end;
  }
  listNode *infoNode;
  if(*found){
    infoNode->start = cascade(bounds_arg->start);
    free(bounds_arg);
    free(bounds_ret);
    switch(lookupMode){
      case 0: return infoNode->vaccinationDate;
      case 1: return infoNode->citizen;
      case 2: return infoNode->citizen->country;
      default: fprintf(stderr, "INVALID LOOKUPMODE IN LOOKUP_SKIPLIST\n");
        break;
    }
  }else{
    free(bounds_arg);
    free(bounds_ret);
    return NULL;
  }
}

void print_skiplist(skipList *s){
  for(int i = s->height - 1; i >= 0; i--){
    printf("LEVEL: %d\n", i);
    print_list(s->levels[i]);
  }
}

//////////////////////////////////////////////////////////////
// In this function, we start from top to bottom            //
// hoping to locate the node in one of the lists quickly.   //
// Once we do, all we must do is follow the trail of bottom //
// pointers to the lowest level, and delete in constant time//
// in each of the remaining lists.                          //
//////////////////////////////////////////////////////////////

void delete_skipnode(skipList *s, int id){
  int mode = 0;
  listNode *result;
  listNode *location = NULL;
  for(int i = s->height - 1; i >= 0; i--){
    result = delete_node(s->levels[i], mode, id, location);
    if(result != NULL){
      mode = 1;
      location = result;
    }
  }
}


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