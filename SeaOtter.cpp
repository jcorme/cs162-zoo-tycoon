#include "SeaOtter.h"
#include "Utils.h"

SeaOtter::SeaOtter(unsigned age):
    Animal("Sea Otter", age, SEA_OTTER_UNIT_COST,
           SEA_OTTER_BABIES_PER_BIRTH,
           SEA_OTTER_FOOD_COST_MULTIPLIER) {}

AnimalsVec SeaOtter::GiveBirth() const {
  return AnimalGiveBirth<SeaOtter>(this);
}
