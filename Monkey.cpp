/*********************************************************************
** Program Filename: Monkey.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the Monkey class and in
 * the Monkey header.
** Input: None
** Output: None
*********************************************************************/
#include "Monkey.h"

/*********************************************************************
** Function: Monkey
** Description: Constructor for the Monkey class.
** Parameters: age is the age of the new Monkey object.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Monkey::Monkey(unsigned age):
    Animal("Monkey", age, MONKEY_UNIT_COST,
           MONKEY_BABIES_PER_BIRTH,
           MONKEY_FOOD_COST_MULTIPLIER,
           MONKEY_REVENUE_PCT) {}

/*********************************************************************
** Function: DailyRevenue
** Description: Calculates the daily revenue generated by the given monkey.
** Parameters: bonus_revenue is an optional amount of extra revenue the
 * monkey should generate.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
double Monkey::DailyRevenue(Option<unsigned> bonus_revenue) const {
  double revenue = Animal::DailyRevenue(None);
  revenue += bonus_revenue.UnwrapOr(0);
  return revenue;
}

/*********************************************************************
** Function: GiveBirth
** Description: Gives birth to babies_per_birth() number of new monkeys.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
AnimalsVec Monkey::GiveBirth() const {
  return AnimalGiveBirth<Monkey>(this);
}
