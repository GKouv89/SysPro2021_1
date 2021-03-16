#include <stdio.h>
#include <stdlib.h>

#include "../include/bucketlist.h"
#include "../include/country.h"
#include "../include/citizen.h"
#include "../include/virus.h"

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
  if(bl->type == Citizen_List){
    update_population((Country *) ((Citizen *)content)->country, ((Citizen *)content)->age);
  }
}

void* search_bucketList(bucketList *bl, char *str){
  bucketNode *temp = bl->front;
  while(temp){
    if(bl->type == Country_List){
      if(isEqual_country(temp->content, str)){
        return temp->content;
      }
    }else if(bl->type == Virus_List){
      if(isEqual_virus(temp->content, str)){
        return temp->content;
      }
    }else if(bl->type == Citizen_List){
      if(isEqual_citizen(temp->content, str)){
        return temp->content;
      }
    }
    temp = temp->next;
  }
  return NULL;
}

void vacStatus_all(bucketList *bl, unsigned char *citizenID){
  if(bl->type == Virus_List){
    bucketNode *temp = bl->front;
    listNode *res;
    while(temp){
      res = lookup_in_virus_vaccinated_for_list((Virus *)temp->content, atoi(citizenID));
      if(res == NULL){
        printf("%s NO\n", ((Virus *)temp->content)->name);
      }else{
        printf("%s YES %s\n", ((Virus *)temp->content)->name, res->vaccinationDate);
      }
      temp = temp->next;
    }
  }
}

void popStatus_all(bucketList *bl, int mode, Virus *v, char *startingDate, char *endingDate){
  if(bl->type == Country_List){
    bucketNode *temp = bl->front;
    while(temp){
      population *pop = skiplist_vac_status_country(v->vaccinated_for, 1, (Country *)temp->content, startingDate, endingDate);
      population *popNo = skiplist_vac_status_country(v->not_vaccinated_for, 0, (Country *)temp->content, startingDate, endingDate);
      if(mode == 0){
        printf("%s ", ((Country *)temp->content)->name);
        int overallVac = 0;
        int overallPop = 0;
        for(int i = 0; i < 4; i++){
          overallVac += pop->popByAgeGroup[i];
          overallPop += pop->popByAgeGroup[i] + popNo->popByAgeGroup[i];
        }
        if(overallPop == 0 && overallVac == 0){
          printf("0 0%%\n");
        }else{
          printf("%d %.2lf%%\n", overallVac, ((double)overallVac/(double)overallPop)*100);
        }
      }else{
        printf("%s ", ((Country *)temp->content)->name);
        int overallPop[4] = {0, 0, 0, 0};
        for(int i = 0; i < 4; i++){
          overallPop[i] += pop->popByAgeGroup[i] + popNo->popByAgeGroup[i];
        }
        if(overallPop[0] == 0){
          printf("0-20 0 0%%\n");
        }else{
          printf("0-20 %d %.2lf%%\n", pop->popByAgeGroup[0], ((double)pop->popByAgeGroup[0]/(double)overallPop[0])*100);
        }
        if(overallPop[1] == 0){
          printf("21-40 0 0%%\n");
        }else{
          printf("21-40 %d %.2lf%%\n", pop->popByAgeGroup[1], ((double)pop->popByAgeGroup[1]/(double)overallPop[1])*100);
        }
        if(overallPop[2] == 0){
          printf("41-60 0 0%%\n");
        }else{
          printf("41-60 %d %.2lf%%\n", pop->popByAgeGroup[2], ((double)pop->popByAgeGroup[2]/(double)overallPop[2])*100);
        }
        if(overallPop[3] == 0){
          printf("60+ 0 0%%\n");
        }else{
          printf("60+ %d %.2lf%%\n", pop->popByAgeGroup[3], ((double)pop->popByAgeGroup[3]/(double)overallPop[3])*100);
        }
      }
      free(pop);
      free(popNo);  
      temp = temp->next;
    }
  }
}

void destroy_bucketList(bucketList **bl){
  bucketNode *temp = (*bl)->front;
  bucketNode *to_del;
  while(temp){
    to_del = temp;
    temp = temp->next;
    if((*bl)->type == Country_List){
      Country *to_destroy = (Country *)to_del->content;
      destroy_country(&to_destroy);
    }else if((*bl)->type == Virus_List){
      Virus *to_destroy = (Virus *)to_del->content;
      destroy_virus(&to_destroy);
    }else if((*bl)->type == Citizen_List){
      Citizen *to_destroy = (Citizen *)to_del->content;
      destroy_citizen(&to_destroy);
    }
    free(to_del);
    to_del = NULL;
  }
  free(*bl);
  *bl = NULL;
}
