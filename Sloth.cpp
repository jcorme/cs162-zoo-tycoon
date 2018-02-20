/*********************************************************************
** Program Filename: Sloth.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the Sloth class and in
 * the Sloth header.
** Input: None
** Output: None
*********************************************************************/
#include "Sloth.h"
#include "Utils.h"

/*********************************************************************
** Function: Sloth
** Description: Constructor for the Sloth class.
** Parameters: age is the age of the new Sloth object.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Sloth::Sloth(unsigned age):
    Animal("Sloth", age, SLOTH_UNIT_COST,
           SLOTH_BABIES_PER_BIRTH,
           SLOTH_FOOD_COST_MULTIPLIER) {}

/*********************************************************************
** Function: GiveBirth
** Description: Gives birth to babies_per_birth new sloths.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
AnimalsVec Sloth::GiveBirth() const {
  return AnimalGiveBirth<Sloth>(this);
}
