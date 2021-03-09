#ifndef COMMANDS_H
#define COMMANDS_H

#include "hashmap.h"
#include "virus.h"
#include "citizen.h"

void vaccineStatusBloom(hashMap *, unsigned char *, unsigned char *);
void vaccineStatus(hashMap *, unsigned char *, unsigned char *);
void vaccineStatusAll(hashMap *, unsigned char *);
void vaccinateNow(Virus *, unsigned char *, unsigned char *, Citizen *);
void insertCitizenRecord(hashMap *, hashMap *, hashMap *, int, char *);
void list_non_vaccinated_persons(hashMap *, unsigned char *);
void popStatusByAge(hashMap *, hashMap *, unsigned char *, unsigned char *, char *, char *);
#endif