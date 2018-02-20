#ifndef ZOO_TYCOON_ELEPHANT_H
#define ZOO_TYCOON_ELEPHANT_H
/*********************************************************************
** Program Filename: Elephant.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the Elephant class and its related members. 
** Input: None
** Output: None
*********************************************************************/


#include "Animal.h"

static constexpr unsigned ELEPHANT_UNIT_COST = 24000;
static constexpr unsigned ELEPHANT_BABIES_PER_BIRTH = 1;
static constexpr unsigned ELEPHANT_FOOD_COST_MULTIPLIER = 8;
static constexpr double ELEPHANT_REVENUE_PCT = 0.16;

class Elephant: public Animal {
  public:
    explicit Elephant(unsigned age);
    Elephant() : Elephant(0) {}

    virtual std::vector<std::unique_ptr<Animal>> GiveBirth() const override;
};


#endif //ZOO_TYCOON_ELEPHANT_H
