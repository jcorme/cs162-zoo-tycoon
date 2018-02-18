#ifndef ZOO_TYCOON_SEAOTTER_H
#define ZOO_TYCOON_SEAOTTER_H


#include "Animal.h"

static constexpr unsigned SEA_OTTER_UNIT_COST = 5000;
static constexpr unsigned SEA_OTTER_BABIES_PER_BIRTH = 2;
static constexpr unsigned SEA_OTTER_FOOD_COST_MULTIPLIER = 2;

class SeaOtter: public Animal
{
  public:
    explicit SeaOtter(unsigned age);
    SeaOtter(): SeaOtter(0) {}

    virtual AnimalsVec GiveBirth() const override;
};


#endif //ZOO_TYCOON_SEAOTTER_H
