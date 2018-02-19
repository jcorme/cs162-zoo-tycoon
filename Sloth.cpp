#include "Sloth.h"
#include "Utils.h"

Sloth::Sloth(unsigned age):
    Animal("Sloth", age, SLOTH_UNIT_COST,
           SLOTH_BABIES_PER_BIRTH,
           SLOTH_FOOD_COST_MULTIPLIER) {}

AnimalsVec Sloth::GiveBirth() const {
  return AnimalGiveBirth<Sloth>(this);
}
