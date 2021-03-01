#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/linkedlist.h"

void create_list(list ** l){
  (*l) = malloc(sizeof(list));
  (*l)->front = NULL;
  (*l)->rear = NULL;
}

listNode* insert_node(list *l, listNode *startingNode, int id, char *vacDate, Citizen *c){
  listNode *temp = startingNode;
  #ifdef DEBUG
  if(startingNode != NULL){
    printf("startingNode: %d\n", startingNode->id);
  }
  #endif
  listNode *newNode = malloc(sizeof(listNode));
  newNode->id = id;
  if(vacDate != NULL){
    newNode->vaccinationDate = malloc((strlen(vacDate) + 1)*sizeof(char));
    strcpy(newNode->vaccinationDate, vacDate);
  }else{
    newNode->vaccinationDate = NULL;
  }
  newNode->citizen = c;
  newNode->bottom = NULL; // THIS MUST CHANGE
  // First case: list is empty
  if(l->front == NULL && l->rear == NULL){
    #ifdef DEBUG
    printf("INSERT 1ST CASE: %d\n", id);
    #endif
    l->front = newNode;
    l->rear = newNode;
    newNode->next = newNode->prev = NULL;
  }else if(l->rear == startingNode){ // Second case: startingNode is the list's rear node
    // this means the new node will be the new rear
    #ifdef DEBUG
    printf("INSERT 2ND CASE: %d\n", id);
    #endif
    newNode->prev = l->rear;
    newNode->next = NULL;
    l->rear->next = newNode;
    l->rear = newNode;
  }else if(l->front != NULL && startingNode == NULL){ 
    // Third case: startingNode is NULL, meaning the new node has a value smaller than
    // the front node's value
    #ifdef DEBUG
    printf("INSERT 3RD CASE: %d\n", id);
    #endif
    newNode->prev = NULL;
    newNode->next = l->front;
    l->front->prev  = newNode;
    l->front = newNode;
  }else{ //The node is somewhere in the middle
    #ifdef DEBUG
    printf("INSERT 4TH CASE: %d\n", id);
    #endif
    while(temp != NULL){
      if(id == temp->id){
        // Node already exists
        // In none of the other three cases is this a problem, as we can specifically check
        // The values of l->front or l->rear
        free(newNode);
        newNode = NULL;
        break;
      }else if(id > temp->id){
        // 2 cases
        // temp has a next node, therefore a second constraing must be met
        // or simply temp is greater than all list nodes
        if(temp->next == NULL || (temp->next != NULL && id < temp->next->id)){
          newNode->prev = temp;
          newNode->next = temp->next;
          if(temp->next != NULL){
            temp->next->prev = newNode;
          }else{
            l->rear = newNode;
          }
          temp->next = newNode;
          break;          
        }
      }
      temp = temp->next;
    }
  }
  return newNode;
}

void print_list(list *l){
  listNode *temp = l->front;
  while(temp){
    printf("%d\t", temp->id);
    temp = temp->next;
  }
  printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
// In delete we require these arguments                                       //
// Search mode: we have not yet located the node in any of the lists so far   //
// So we still search for it                                                  //
// Located mode: We have located the node in one of the upper lists           //
// and we have been following the trail of bottom pointers to the current list//
// we know which node to delete                                               //
// Node: NULL in the first case, a pointer to the node we have located in the //
// second                                                                     //
// id: the same in both cases, but in located we ignore it                    //  
////////////////////////////////////////////////////////////////////////////////

listNode* delete_node(list *l, int mode, int id, listNode *located){
  listNode *temp;
  listNode *to_ret, *to_del;
  int found = 0;
  if(mode == 0){ // Searching for the node
    temp = l->front;
    while(temp){
      if(temp->id == id){ // Node was just located
        found = 1;
        to_ret = temp->bottom;
        to_del = temp;
        break;
      }
      temp = temp->next;
    }
    // If we have reached this point and not found the node
    // the node is probably in a lower level, but not here
    // so we return null
    if(found == 0){
      to_ret = NULL;
      return to_ret;
    }
  }else{ // Node located
    to_del = located;
    to_ret = to_del->bottom;
  }
  if(to_del == l->front && to_del == l->rear){
    // First case: the list will be empty after this
    l->front = l->rear = NULL;
  }else if(to_del == l->front && l->front != l->rear){
    // Second case: we must change only l->front
    to_del->next->prev = NULL;
    l->front = to_del->next;
  }else if(to_del == l->rear && l->front != l->rear){
    // Third case: we must change only l->rear
    to_del->prev->next = NULL;
    l->rear = to_del->prev;
  }else{
    // Fourth case: node is somewhere in the middle
    to_del->prev->next = to_del->next;
    to_del->next->prev = to_del->prev;
  }
  to_del->bottom = to_del->prev = to_del->next = NULL;
  to_del->citizen = NULL;
  if(to_del->vaccinationDate != NULL){
    free(to_del->vaccinationDate);
    to_del->vaccinationDate = NULL;
  }
  free(to_del);
  to_del = temp = NULL;
  return to_ret;
}

void search(list *l, int id, listNode *startingNode, listNode *endingNode, boundaries **new_bound, int *error, listNode **futureSN){
  listNode *temp;  
  //In all cases, we return:
  // A pair of boundaries that will act as guidelines for search on the bottom list
  // We also change the content of startingNode before we return
  // which will point to the node after which the new one should be inserted 
  if(startingNode == NULL && endingNode == NULL){
    #ifdef DEBUG
    printf("FIRST CASE: %d\n", id);
    #endif
    // First case: list is empty (this happens at the very start of our skip list instance)
    (*new_bound)->start = (*new_bound)->end = NULL;
    *error = 0;
    // startingNode would be NULL but it already is so no changes necessary
    *futureSN = NULL;
    return;
  }else if(startingNode == NULL && endingNode != NULL){
    #ifdef DEBUG
    printf("SECOND CASE: %d\n", id);
    #endif
    // Second case: list is non empty and because the id is smaller than
    // the above list's head's id, we must start from endingNode in the current list
    // and work backwards
    temp = endingNode;
    while(temp != startingNode){
      if(id == temp->id){
        (*new_bound)->start = (*new_bound)->end = NULL;
        *error = 1;
        return;
      }
      if(id > temp->id){
        *error = 0;
        *futureSN = temp;
        (*new_bound)->start = temp->bottom;
        (*new_bound)->end = temp->next->bottom;
        #ifdef DEBUG
        if(*futureSN == NULL){
          printf("futureSN is NULL\n");
        }else{
          printf("futureSN->id = %d\n", (*futureSN)->id);
        }
        #endif
        return;
      }    
      temp = temp->prev;
    }
    // Ιf we have reached this point without a return statement
    // the id is smaller than any in the list already
    *futureSN = NULL;
    (*new_bound)->start = NULL;
    (*new_bound)->end = l->front->bottom;
    #ifdef DEBUG
    if(*futureSN == NULL){
      printf("futureSN is NULL\n");
    }else{
      printf("futureSN->id = %d\n", (*futureSN)->id);
    }
    #endif
    return;
  }else if(startingNode != NULL && endingNode == NULL){
    #ifdef DEBUG
    printf("THIRD CASE: %d\n", id);
    #endif
    // Third case: we know the id is greater than a certain node,
    // but this node's id was the rear's id in the list above the current one
    temp = startingNode;
    while(temp){
      if(id == temp->id){
        (*new_bound)->start = (*new_bound)->end = NULL;
        *error = 1;
        return;
      }
      if(id < temp->id){
        *error = 0;
        *futureSN = temp->prev;
        if(temp->prev){
          (*new_bound)->start = temp->prev->bottom;
        }else{
          (*new_bound)->start = NULL;
        }
        (*new_bound)->end = temp->bottom;
        #ifdef DEBUG
        if(*futureSN == NULL){
          printf("futureSN is NULL\n");
        }else{
          printf("futureSN->id = %d\n", (*futureSN)->id);
        }
        #endif
        return;
      }   
      temp = temp->next;
    }
    // Ιf we have reached this point without a return statement
    // the id is greater than any in the list already
    // In this case, the search must take place from the bottom of the list's rear
    // until the rear of the list underneath the current one.
    *futureSN = l->rear;
    (*new_bound)->start = l->rear->bottom;
    (*new_bound)->end = NULL;
    #ifdef DEBUG
    if(*futureSN == NULL){
      printf("*futureSN is NULL\n");
    }else{
      printf("*futureSN->id = %d\n", (*futureSN)->id);
    }
    #endif
    return;
  }else if(startingNode == endingNode){
    #ifdef DEBUG
    printf("FOURTH CASE: %d\n", id);
    #endif
    // Fourth case: list has only one member
    temp = startingNode;
    if(id < temp->id){
      #ifdef DEBUG
      printf("CASE 4.1 \n");
      #endif
      (*new_bound)->start = NULL;
      (*new_bound)->end = l->front->bottom;
      *futureSN = NULL; // this corresponds to line 27
      *error = 0;
    }else if(id == temp->id){
      (*new_bound)->start = (*new_bound)->end = NULL;
      *futureSN = NULL;
      *error = 1;
    }else{
      #ifdef DEBUG
      printf("CASE 4.2\n");
      #endif
      *error = 0;
      *futureSN = l->rear;
      (*new_bound)->start = l->rear->bottom;
      (*new_bound)->end = NULL;
    }
    #ifdef DEBUG
    if(*futureSN == NULL){
      printf("*futureSN is NULL\n");
    }else{
      printf("*futureSN->id = %d\n", (*futureSN)->id);
    }
    #endif
    return;
  }else{
    #ifdef DEBUG
    printf("FIFTH CASE: %d\n", id);
    #endif
    temp = startingNode;
    // Fifth case: multiple members, and strict bounds on both sides.
    while(temp != endingNode->next){
      if(id == temp->id){
        (*new_bound)->start = (*new_bound)->end = NULL;
        *error = 1;
        return;
      }
      if(id < temp->id){
        *error = 0;
        *futureSN = temp->prev;
        if(temp->prev){
          (*new_bound)->start = temp->prev->bottom;
        }else{
          (*new_bound)->start = NULL;
        }
        (*new_bound)->end = temp->bottom;
        #ifdef DEBUG
        if(*futureSN == NULL){
          printf("*futureSN is NULL\n");
        }else{
          printf("*futureSN->id = %d\n", (*futureSN)->id);
        }
        #endif
        return;
      }    
      temp = temp->next;
    }
    // If we have reached this point without any return statements,
    // we are probably at the highest of our lists and 
    // the number is greater than all inserted so far.
    *futureSN = l->rear;
    (*new_bound)->start = l->rear->bottom;
    (*new_bound)->end = NULL;
    #ifdef DEBUG
    if(*futureSN == NULL){
      printf("*futureSN is NULL\n");
    }else{
      printf("*futureSN->id = %d\n", (*futureSN)->id);
    }
    #endif
    return;    
  }
}
void destroy_list(list **l){
  if(*l == NULL){
    return;
  }
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