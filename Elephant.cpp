#include "Elephant.h"

Elephant::Elephant(unsigned age):
    Animal("Elephant", age, ELEPHANT_UNIT_COST,
           ELEPHANT_BABIES_PER_BIRTH, ELEPHANT_FOOD_COST_MULTIPLIER,
           ELEPHANT_REVENUE_PCT) {}

AnimalsVec Elephant::GiveBirth() const {
  return AnimalGiveBirth<Elephant>(this);
}
