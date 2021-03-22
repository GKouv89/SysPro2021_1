#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/skiplist.h"
#include "../include/dateOps.h"
#include "../include/country.h"

void create_skiplist(skipList **s, unsigned long long exp_data_count){
  (*s) = malloc(sizeof(skipList));
  (*s)->height = 1; 
  (*s)->max_height = log2(exp_data_count);
  (*s)->levels = malloc(sizeof(*((*s)->levels)));
  create_list(&((*s)->levels[0]));
}

char* insert_skipnode(skipList *s, int id, char *vacDate, Citizen *c){
  listNode **startingNodes = malloc(s->height*sizeof(listNode *));
  // As we are about to descend into the skiplist's levels,
  // we take note of the point in the list where the value of id
  // became greater than the value of the list node, and therefore where
  // we will move to the next, lower, level.
  // This way, when we decide in how many levels the current id will
  // be inserted into, we will know for each level after which node
  // we should insert the new one.
  int error = 0;
  char *dupeVaccinationDate = search_skip(s, id, startingNodes, &error);
  if(error == 1){
    // ID already exists in skip list
    free(startingNodes);
    return dupeVaccinationDate;
  }
  int idHeight = 1;
  while(rand() % 1000 < 1000*p && idHeight < s->max_height){
    idHeight++;
  }
  if(idHeight > s->height){
    // The skiplist grew, new level(s) will be created.
    list **temp = realloc(s->levels, idHeight*sizeof(*(s->levels)));
    assert(temp != NULL);
    s->levels = temp;
    // Since new level(s) will be added, we also take note of where
    // the new node will be inserted into these lists: NULL means at the
    // very start.
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
  listNode *prevConnection = NULL; 
  // this will connect the node of a list
  // with the corresponding one in the list right above that
  listNode *currConnection;
  
  for(int i = 0; i < s->height ;i++){
    if(i == 0){
      currConnection = insert_node(s->levels[i], startingNodes[i], id, vacDate, c);
    }else{
      currConnection = insert_node(s->levels[i], startingNodes[i], id, NULL, NULL);
    }
    currConnection->bottom = prevConnection;
    prevConnection = currConnection;
    startingNodes[i] = NULL;
  }
  free(startingNodes);
  return NULL;
}

char* search_skip(skipList *s, int id, listNode *startingNodes[], int *error){
  boundaries *bounds_ret = malloc(sizeof(boundaries));
  boundaries *bounds_arg = malloc(sizeof(boundaries));
  // Bounds_arg the nodes between which we will be searching in that specific level
  // For the top level, the entire list is searched.
  // When two nodes whose values are such that first_node_id < id < second_node_id,
  // the search on the next level must take place between the nodes that are 'directly underneath'
  // these two nodes, and so, bounds_ret contains the bottom field of these two nodes.
  // Now, if the id is smaller than the id of the first node in the list/level,
  // then bounds_ret->start will be NULL and bounds_ret->end will be the bottom field of the list's first node.
  // Respectively, if the id is greater than the id of the last node in the list/level,
  // then bounds_ret->start will be the bottom field of the list's last node and bounds_end->end will be NULL,
  // which means in the next level the search will occur from bounds_ret->start and forward.
  bounds_arg->start = s->levels[s->height - 1]->front;
  bounds_arg->end = s->levels[s->height - 1]->rear;
  listNode *futureSN;
  char *dupeVaccinationDate;
  for(int i = s->height - 1; i >= 0; i--){
    dupeVaccinationDate = search(s->levels[i], id, bounds_arg->start, bounds_arg->end, &bounds_ret, error, &futureSN);
    if(*error == 1){ // Element already in skiplist
      startingNodes[i] = NULL;// WIP: what do we insert in startingNodes?
      free(bounds_ret);
      free(bounds_arg);
      return dupeVaccinationDate;
    }
    startingNodes[i] = futureSN; // Keeping track of where the new node will potentially be inserted, if 
    // the node's height reaches the current level.
    // For insertion, for all lists except the bottom one,
    // the startingNode will be the node right after which the new one will be inserted
    bounds_arg->start = bounds_ret->start;
    bounds_arg->end = bounds_ret->end;
  }
  free(bounds_arg);
  free(bounds_ret);
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Search is used to locate where a node will be placed after insertion in a list    //
// Lookup on the other hand is used to locate whether a node is in the skiplist and, //
// if so, provide info on that citizen's vaccination state                           //
///////////////////////////////////////////////////////////////////////////////////////

listNode* lookup_skiplist(skipList *s, int id){
  boundaries *bounds_ret = malloc(sizeof(boundaries));
  boundaries *bounds_arg = malloc(sizeof(boundaries));
  bounds_arg->start = s->levels[s->height - 1]->front;
  bounds_arg->end = s->levels[s->height - 1]->rear;
  listNode *futureSN;
  int found = 0;
  for(int i = s->height - 1; i >= 0; i--){
    search(s->levels[i], id, bounds_arg->start, bounds_arg->end, &bounds_ret, &found, &futureSN);
    // futureSN is ignored here
    if(found == 1){ // Element already in skiplist
      break;
    }
    bounds_arg->start = bounds_ret->start;
    bounds_arg->end = bounds_ret->end;
  }
  listNode *infoNode;
  // if the id was found in one of the list's upper levels (any level that is not the bottom one)
  // the bottom pointers to the bottom list are followed (cascade function), 
  // where the vaccinattionDate is included. 
  if(found){
    infoNode = cascade(bounds_ret->start);
    free(bounds_arg);
    free(bounds_ret);
    return infoNode;
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

void print_skiplist_citizens(skipList *s){
  listNode *bottomList = s->levels[0]->front;
  while(bottomList){
    print_citizen(bottomList->citizen);
    bottomList = bottomList->next;
  }
}

void incrementAgeGroup(int age, struct vaccinationsAgeGroup** vacs, int mode){
  // mode indicates whether we will increase both a range field
  // or just the overall age group population counter  
  // If it is the vaccinated_for skiplist of a virus being traversed,
  // and the citizen's vaccination date is in the range,
  // mode would be 1.
  // If the date is not in the range of the two arguement dates,
  // mode would be 0. Mode would also be 0 if it is the *not*_vaccinated_for
  // skiplist of a virus being traversed.
  if(age <= 20){
    (*vacs)->overall[0]++;
    if(mode == 1){
      (*vacs)->inRange[0]++;
    }
  }else if(age > 20 && age <= 40){
    (*vacs)->overall[1]++;
    if(mode == 1){
      (*vacs)->inRange[1]++;
    }
  }else if(age > 40 && age <= 60){
    (*vacs)->overall[2]++;
    if(mode == 1){
      (*vacs)->inRange[2]++;
    }
  }else{
    (*vacs)->overall[3]++;
    if(mode == 1){
      (*vacs)->inRange[3]++;
    }
  }
}

void* skiplist_vac_status_country(skipList *s, int vacMode, int ageMode, Country *country, char *date1, char *date2){
  // Traversing the bottom list of a skiplist and counting how many citizens come from a specific country.
  // vacMode indicates whether it is a list containing vaccinated or not vaccinated people being traversed. 
  // ageMode indicates whether the counting will be recorded seperately in age groups or cumulatively.
  // If it is a vaccinated persons' list, then seperate counting for people vaccinated between date1 and date2 
  // also takes place.
  listNode *bottomList = s->levels[0]->front;
  void *vacs;
  if(ageMode == 0){
    vacs = malloc(sizeof(struct vaccinations));
    ((struct vaccinations *)vacs)->inRange = 0;
    ((struct vaccinations *)vacs)->overall = 0;
  }else{
    vacs = malloc(sizeof(struct vaccinationsAgeGroup));
    for(int i = 0; i < 4; i++){
      ((struct vaccinationsAgeGroup *)vacs)->inRange[i] = 0;
      ((struct vaccinationsAgeGroup *)vacs)->overall[i] = 0;      
    }
  }
  while(bottomList){
    if(isEqual_country(bottomList->citizen->country, country->name)){
      if(vacMode == 1){
        if(dateComparison(date1, bottomList->vaccinationDate) || dateComparison(bottomList->vaccinationDate, date2)){
          if(ageMode == 0){
            ((struct vaccinations *)vacs)->overall++;
          }else{
            incrementAgeGroup(bottomList->citizen->age, (struct vaccinationsAgeGroup**)&vacs, 0);
          }
          bottomList = bottomList->next;
          continue;
        }else{
          if(ageMode == 0){
            ((struct vaccinations *)vacs)->inRange++;
            ((struct vaccinations *)vacs)->overall++;
          }else{
            incrementAgeGroup(bottomList->citizen->age, (struct vaccinationsAgeGroup**)&vacs, 1);
          }
        }
      }else{
        if(ageMode == 0){
          ((struct vaccinations *)vacs)->overall++;
        }else{
          incrementAgeGroup(bottomList->citizen->age, (struct vaccinationsAgeGroup**)&vacs, 0);
        }
      }
    }
    bottomList = bottomList->next;
  }
  return vacs;
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