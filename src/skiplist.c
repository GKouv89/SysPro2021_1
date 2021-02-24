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
  listNode **startingNodes = malloc(s->height*sizeof(*startingNodes));
  // we will have all the nodes where we went a level lower
  // therefore the boundaries for the insertion into that height's list
  // if the new node will reach that height
  int error;
  search_skip(s, id, &startingNodes, &error);
  if(error == 1){
    // ID already exists in skip list
    return;
  }
  int idHeight = 1;
  while(rand() % 2 == 1){
    idHeight++;
  }
  if(idHeight > s->height){
    list **temp = realloc(s->levels, idHeight*sizeof(*(s->levels)));
    assert(temp != NULL);
    s->levels = temp;
    listNode **temp2 = realloc(startingNodes, idHeight*sizeof(*temp2));
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
  for(int i = 0; i < s->height ;i++){
    currConnection = insert_node(s->levels[i], startingNodes[i], id);
    currConnection->bottom = prevConnection;
    prevConnection = currConnection;
  }
}

void search_skip(skipList *s, int id, listNode ***startingNodes, int *error){
  boundaries *bounds_ret, *bounds_arg = malloc(sizeof(boundaries));
  bounds_arg->start = bounds_arg->end = NULL;
  for(int i = s->height; i >= 0; i--){
    bounds_ret = search(s->levels[i], id, bounds_arg->start, bounds_arg->end, error);
    if(*error == 1){ // Element already in skiplist
      (*startingNodes)[i] = NULL;// WIP: what do we insert in startingNodes?
      return;
    }
    (*startingNodes)[i] = bounds_ret->start; // For insertion, for all lists except the bottom one,
    // the startingNode will be the node right after which the new one will be inserted
    // This is also the reason for which in insertion, we only require a starting node
    bounds_arg->start = bounds_ret->start;
    bounds_arg->end = bounds_ret->end;
  }
}

void print_skiplist(skipList *s){
  for(int i = s->height - 1; i >= 0; i++){
    print_list(s->levels[i]);
  }
}

// void delete_skipnode(skipList *, int);


void destroy_skiplist(skipList **s){
  for(int i = 0; i < (*s)->height; i++){
    destroy_list(&((*s)->levels[0]));
  }
  free((*s)->levels);
  (*s)->levels = NULL;
  free(*s);
  (*s) = NULL;
}