/*********************************************************************
** Program Filename: Elephant.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the Elephant class and
 * in the Elephant header.
** Input: None
** Output: None
*********************************************************************/
#include "Elephant.h"

/*********************************************************************
** Function: Elephant
** Description: Constructor for the Elephant class.
** Parameters: age is the age of the created elephant.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Elephant::Elephant(unsigned age):
    Animal("Elephant", age, ELEPHANT_UNIT_COST,
           ELEPHANT_BABIES_PER_BIRTH, ELEPHANT_FOOD_COST_MULTIPLIER,
           ELEPHANT_REVENUE_PCT) {}

/*********************************************************************
** Function: GiveBirth
** Description: Creates and returns one new elephant.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
AnimalsVec Elephant::GiveBirth() const {
  return AnimalGiveBirth<Elephant>(this);
}
