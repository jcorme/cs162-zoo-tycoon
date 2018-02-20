/*********************************************************************
** Program Filename: SeaOtter.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the SeaOtter class and
 * in the SeaOtter header.
** Input: None
** Output: None
*********************************************************************/
#include "SeaOtter.h"
#include "Utils.h"

/*********************************************************************
** Function: SeaOtter
** Description: Constructor for the SeaOtter class.
** Parameters: age is the age of the new Sea Otter object.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SeaOtter::SeaOtter(unsigned age):
    Animal("Sea Otter", age, SEA_OTTER_UNIT_COST,
           SEA_OTTER_BABIES_PER_BIRTH,
           SEA_OTTER_FOOD_COST_MULTIPLIER) {}

/*********************************************************************
** Function: GiveBirth
** Description: Gives birth to babies_per_birth() new sea otters.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
AnimalsVec SeaOtter::GiveBirth() const {
  return AnimalGiveBirth<SeaOtter>(this);
}
