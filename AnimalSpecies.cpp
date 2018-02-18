#include "AnimalSpecies.h"

const ActionStringMap<AnimalSpecies> AnimalSpeciesToStringMap = {
    { AnimalSpecies::Monkey, "Monkey" },
    { AnimalSpecies::SeaOtter, "Sea Otter" },
    { AnimalSpecies::Sloth, "Sloth" },
    { AnimalSpecies::Elephant, "Elephant" }
};

std::string AnimalSpeciesToString(AnimalSpecies s)
{
  return AnimalSpeciesToStringMap.at(s);
}

std::unique_ptr<Animal> CreateFromSpecies(AnimalSpecies s, unsigned age)
{
  switch (s)
  {
    case AnimalSpecies::Monkey:
      return std::move(CreateDynamicAnimal<AnimalSpecies::Monkey>()(1, age)[0]);
    case AnimalSpecies::SeaOtter:
      return std::move(
          CreateDynamicAnimal<AnimalSpecies::SeaOtter>()(1, age)[0]);
    case AnimalSpecies::Sloth:
      return std::move(CreateDynamicAnimal<AnimalSpecies::Sloth>()(1, age)[0]);
    case AnimalSpecies::Elephant:
      return std::move(
          CreateDynamicAnimal<AnimalSpecies::Elephant>()(1, age)[0]);
  }
}

std::vector<AnimalSpecies> AllSpecies()
{
  return ActionStringMapKeys(AnimalSpeciesToStringMap);
}
