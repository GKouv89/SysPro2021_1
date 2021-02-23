#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void create_list(list ** l){
  (*l) = malloc(sizeof(list));
  l->front = NULL;
  l->rear = NULL;
}

listNode* insert_node(list *l, listNode *startingNode, int id){
  listNode *temp = startingNode;
  listNode *newNode = malloc(sizeof(listNode));
  newNode->id = id;
  newNode->bottom = NULL; // THIS MUST CHANGE
  // First case: list is empty
  if(l->front == NULL && l->rear == NULL){
    l->front = newNode;
    l->rear = newNode;
    newNode->next = newNode->prev = NULL;
  }else if(l->rear == startingNode){ // Second case: startingNode is the list's rear node
    // this means the new node will be the new rear
    newNode->prev = l->rear;
    newNode->next = NULL;
    l->rear->next = newNode;
    l->rear = newNode;
  }else if(l->front != NULL && startingNode == NULL){ 
    // Third case: startingNode is NULL, meaning the new node has a value smaller than
    // the front node's value
    newNode->prev = NULL;
    newNode->next = l->front;
    l->front->prev  = newNode;
    l->front = newNode;
  }else{ //The node is somewhere in the middle
    while(temp != NULL){
      if(id == temp->id){
        // Node already exists
        // In none of the other three cases is this a problem, as we can specifically check
        // The values of l->front or l->rear
        break;
      }else if(id > temp->id && id < temp->next->id){
        newNode->prev = temp;
        newNode->next = temp->next;
        temp->next->prev = newNode;
        temp->next = newNode;
        break;
      }
      temp = temp->next;
    }
  }
  return newNode;
}

// void delete_node(list *, int);
boundaries* search(list *l, int id, listNode *startingNode, listNode *endingNode, int *error){
  boundaries *new_bound = malloc(sizeof(boundaries));
  if(startingNode == NULL && endingNode == NULL){
    // First case: list is empty (this happens at the very start of our skip list instance)
    new_bound->start = new_bound->end = NULL;
    *error = 0;
    return new_bound;
  }
  listNode *temp = startingNode;  
  while(temp != endingNode){
    if(id == temp->id){
      new_bound->start = new_bound->end = NULL;
      *error = 1;
      return new_bound;
    }
    if(id < temp->id){
      *error = 0;
      new_bound->start = temp->prev->bottom;
      new_bound->end = temp->bottom;
      return new_bound;
    }    
  }
}
void destroy_list(list **l){
  listNode *temp = (*l)->front;
  listNode *to_del;
  while(temp){
    to_del = temp;
    temp = temp->next;
    to_del->prev = to_del->next = to_del->bottom = NULL;
    free(to_del);
  }
  to_del = NULL;
  (*l)->front = (*l)->rear = NULL;
  free(*l);
  *l = NULL;
}