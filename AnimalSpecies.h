#ifndef ZOO_TYCOON_ANIMALSPECIES_H
#define ZOO_TYCOON_ANIMALSPECIES_H


#include "Utils.h"
#include "Monkey.h"
#include "SeaOtter.h"
#include "Sloth.h"
#include "Elephant.h"

enum class AnimalSpecies
{
    Monkey,
    SeaOtter,
    Sloth,
    Elephant,
};

template <AnimalSpecies A>
struct SpeciesToType;

template <>
struct SpeciesToType<AnimalSpecies::Monkey> { using type = Monkey; };

template <>
struct SpeciesToType<AnimalSpecies::SeaOtter> { using type = SeaOtter; };

template <>
struct SpeciesToType<AnimalSpecies::Sloth> { using type = Sloth; };

template <>
struct SpeciesToType<AnimalSpecies::Elephant> { using type = Elephant; };

template <class T>
struct CreateAnimalD
{
  std::vector<std::unique_ptr<T>> operator()(unsigned qty, unsigned age)
  {
    std::vector<std::unique_ptr<T>> ts;
    ts.reserve(qty);
    for (decltype(ts.size()) i = 0; i != qty; ++i)
      ts.push_back(make_unique<T>(T(age)));
    return ts;
  }
};

template <class T>
struct CreateAnimalS
{
  std::vector<T> operator()(unsigned qty, unsigned age)
  {
    std::vector<T> ts;
    ts.reserve(qty);
    for (auto i = 0; i != qty; ++i)
      ts.push_back(T(age));
    return ts;
  }
};

template <AnimalSpecies A>
using CreateDynamicAnimal = CreateAnimalD<typename SpeciesToType<A>::type>;

template <AnimalSpecies A>
using CreateStaticAnimal = CreateAnimalS<typename SpeciesToType<A>::type>;

extern const ActionStringMap<AnimalSpecies> AnimalSpeciesToStringMap;

std::string AnimalSpeciesToString(AnimalSpecies s);

std::unique_ptr<Animal> CreateFromSpecies(AnimalSpecies s, unsigned age);

std::vector<AnimalSpecies> AllSpecies();


#endif //ZOO_TYCOON_ANIMALSPECIES_H
