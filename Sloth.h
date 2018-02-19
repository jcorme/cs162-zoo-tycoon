#ifndef ZOO_TYCOON_SLOTH_H
#define ZOO_TYCOON_SLOTH_H


#include "Animal.h"

static constexpr unsigned SLOTH_UNIT_COST = 2000;
static constexpr unsigned SLOTH_BABIES_PER_BIRTH = 3;
static constexpr unsigned SLOTH_FOOD_COST_MULTIPLIER = 1;

class Sloth: public Animal {
  public:
    explicit Sloth(unsigned age);
    Sloth(): Sloth(0) {}

    virtual AnimalsVec GiveBirth() const override;
};


#endif //ZOO_TYCOON_SLOTH_H
