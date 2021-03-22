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
}

//////////////////////////////////////////////////////////
// The search function called from the find_node        //
// function of the hashmap module. According to the type//
// of bucketlist (and therefore, the type of hashmap)   //
// the appropriate comparison function is called.       //
//////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////
// The command that is called when vaccineStatus is called   //
// without specifying the virus. Called for the list of every//
// hash bucket, for every node of the list (every virus)     //
// a search through the skiplist containing the vaccinated   //
// people for the virus occurs.                              //
///////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////
// The command that is called when popStatus or popStatusByAge is called    //
// without specifying the country. Called for the list of every             //
// hash bucket of the countries hashmap, for every node of the list         //
// (every country) the function that finds the appropriate population counts//
// and ratios for the virus is called with said country as an argument.     //
// ageMode indicates whether popStatus or popStatusByAge is called.         //
// Depending on that, not only the appropriate printing function is called, //
// but the result of the skiplist population search is casted               //
// with the correct type.                                                   //
//////////////////////////////////////////////////////////////////////////////

void popStatus_all(bucketList *bl, int ageMode, Virus *v, char *startingDate, char *endingDate){
  if(bl->type == Country_List){
    bucketNode *temp = bl->front;
    while(temp){
      void *vacced = skiplist_vac_status_country(v->vaccinated_for, 1, ageMode, (Country *)temp->content, startingDate, endingDate);
      void *notVacced = skiplist_vac_status_country(v->not_vaccinated_for, 0, ageMode, (Country *)temp->content, startingDate, endingDate);
      if(ageMode == 0){
        print_vaccination_ratio((Country *)temp->content, (struct vaccinations *)vacced, (struct vaccinations *)notVacced);
      }else{
        print_vaccination_ratios_byAge((Country *)temp->content, (struct vaccinationsAgeGroup *)vacced, (struct vaccinationsAgeGroup *)notVacced);
      }
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
