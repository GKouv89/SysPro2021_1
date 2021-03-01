#include <stdio.h>
#include <stdlib.h>

#include "../include/bucketlist.h"
#include "../include/entities.h"

void create_bucketList(bucketList **bl, typeOfList type){
  (*bl) = malloc(sizeof(bucketList));
  (*bl)->front = (*bl)->rear = NULL;
  (*bl)->type = type;
}

void insert_bucketNode(bucketList *bl, void *content){
  bucketNode *new_node = malloc(sizeof(bucketNode));
  new_node->content = content;
  new_node->next = NULL;
  if(bl->front == NULL && bl->rear == NULL){
    bl->front = new_node;
    bl->rear = new_node;
  }else{
    bl->rear->next = new_node;
    bl->rear = new_node;
  }
}

void* search_bucketList(bucketList *bl, char *str){
  bucketNode *temp = bl->front;
  while(temp){
    if(bl->type == Country){
      if(isEqual_country(temp->content, str)){
        return temp->content;
      }
    }else if(bl->type == Virus){
      if(isEqual_virus(temp->content, str)){
        return temp->content;
      }
    }else if(bl->type == Citizen){
      if(isEqual_citizen(temp->content, str)){
        return temp->content;
      }
    }
    temp = temp->next;
  }
  return NULL;
}

void destroy_bucketList(bucketList **bl){
  bucketNode *temp = (*bl)->front;
  bucketNode *to_del;
  while(temp){
    to_del = temp;
    temp = temp->next;
    if((*bl)->type == Country){
      destroy_country(&(to_del->content));
    }else if((*bl)->type == Virus){
      destroy_virus(&(to_del->content));
    }else if((*bl)->type == Citizen){
      destroy_citizen(&(to_del->content));
    }
    free(to_del);
    to_del = NULL;
  }
  free(*bl);
  *bl = NULL;
}
