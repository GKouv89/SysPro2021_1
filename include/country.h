#ifndef COUNTRY_H
#define COUNTRY_H

typedef struct country{
  char *name;
} Country;

typedef struct pops{
  unsigned int popByAgeGroup[4];  
} population;

////////////////////////////////////////////////////////////////////
// This datatype is used for the popStatus command.               //
// Since there will be two calls of the same skiplist function,   //
// one for the vaccinated-persons skiplist and one for the        //
// non-vaccinated, two of these structs will be utilized.         //
// The inRange field is used only in the struct where the results //
// of the call to the vaccinated-persons skiplist will be stored. //
// It refers to people in that skiplist from this specific country//
// the call was made for whose vaccination dates are in range of  //
// the ones given. Overall is used in both structs, and it stores //
// the number of people in that given skiplist that come from     //
// the country-argument.                                          //
////////////////////////////////////////////////////////////////////

struct vaccinations{
  unsigned int inRange;
  unsigned int overall;
};

///////////////////////////////////////////////////////////////
// This datatype is used for the popStatusByAge command.     //
// It is completely similar to the one above, except here    //
// populations are tracked according to the age range of the //
// vaccinated (or not vaccinated) person.                    //
///////////////////////////////////////////////////////////////

struct vaccinationsAgeGroup{
  unsigned int inRange[4];
  unsigned int overall[4];
};

Country* create_country(const char *);
int isEqual_country(Country *, unsigned char *);
void print_vaccination_ratios_byAge(Country *, struct vaccinationsAgeGroup *, struct vaccinationsAgeGroup *);
void print_vaccination_ratio(Country *, struct vaccinations *, struct vaccinations *);
void destroy_country(Country **);

#endif