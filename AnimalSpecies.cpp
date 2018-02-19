#include "AnimalSpecies.h"

const ActionStringMap<AnimalSpecies> AnimalSpeciesToStringMap = {
  { AnimalSpecies::Monkey, "Monkey" },
  { AnimalSpecies::SeaOtter, "Sea Otter" },
  { AnimalSpecies::Sloth, "Sloth" },
  { AnimalSpecies::Elephant, "Elephant" }
};

std::vector<AnimalSpecies> AllSpecies() {
  return ActionStringMapKeys(AnimalSpeciesToStringMap);
}

std::string AnimalSpeciesToString(AnimalSpecies s) {
  return AnimalSpeciesToStringMap.at(s);
}

#define ZT_SWITCH_NEW_DYNAMIC_ANIMAL(A) \
case AnimalSpecies::A: \
  return std::move(CreateDynamicAnimal<AnimalSpecies::A>()(1, age)[0]);

std::unique_ptr<Animal> CreateFromSpecies(AnimalSpecies s, unsigned age) {
  switch (s) {
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(Monkey);
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(SeaOtter);
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(Sloth);
    ZT_SWITCH_NEW_DYNAMIC_ANIMAL(Elephant);
  }
}

#undef ZT_SWITCH_NEW_DYNAMIC_ANIMAL
