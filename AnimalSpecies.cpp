/*********************************************************************
** Program Filename: AnimalSpecies.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements members and functions declared in the
 * AnimalSpecies header.
** Input: None
** Output: None
*********************************************************************/
#include "AnimalSpecies.h"

// Maps the species enumeration values to their string representations.
const ActionStringMap<AnimalSpecies> AnimalSpeciesToStringMap = {
  { AnimalSpecies::Monkey, "Monkey" },
  { AnimalSpecies::SeaOtter, "Sea Otter" },
  { AnimalSpecies::Sloth, "Sloth" },
  { AnimalSpecies::Elephant, "Elephant" }
};

/*********************************************************************
** Function: AllSpecies
** Description: Returns all possible AnimalSpecies enumeration values.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::vector<AnimalSpecies> AllSpecies() {
  return ActionStringMapKeys(AnimalSpeciesToStringMap);
}

/*********************************************************************
** Function: AnimalSpeciesToString
** Description: Converts the given species to its string representation.
** Parameters: s is the AnimalSpecies value to convert.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::string AnimalSpeciesToString(AnimalSpecies s) {
  return AnimalSpeciesToStringMap.at(s);
}

// Macro solely to reduce DRY.
// Undefined right after usage.
#define ZT_SWITCH_NEW_DYNAMIC_ANIMAL(A) \
case AnimalSpecies::A: \
  return std::move(CreateDynamicAnimal<AnimalSpecies::A>()(1, age)[0]);

/*********************************************************************
** Function: CreateFromSpecies
** Description: Dynamically allocates an Animal subclass of the given
 * species.
** Parameters: s is the species of Animal to create; age is the age the
 * animal should have.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::unique_ptr<Animal> CreateFromSpecies(AnimalSpecies s, unsigned age) {
  switch (s) {
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(Monkey);
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(SeaOtter);
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(Sloth);
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(Elephant);
  }
}

#undef ZT_SWITCH_NEW_DYNAMIC_ANIMAL
