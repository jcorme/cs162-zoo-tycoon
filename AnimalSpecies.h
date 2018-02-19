#ifndef ZOO_TYCOON_ANIMALSPECIES_H
#define ZOO_TYCOON_ANIMALSPECIES_H


#include "Utils.h"
#include "Monkey.h"
#include "SeaOtter.h"
#include "Sloth.h"
#include "Elephant.h"

enum class AnimalSpecies {
  Monkey,
  SeaOtter,
  Sloth,
  Elephant,
};

template <AnimalSpecies A>
struct SpeciesToType;

#define ZT_SPECIALIZE_SPECIES(A, T) \
template <> struct SpeciesToType<AnimalSpecies::A> { using type = T; };

ZT_SPECIALIZE_SPECIES(Monkey, Monkey);
ZT_SPECIALIZE_SPECIES(SeaOtter, SeaOtter);
ZT_SPECIALIZE_SPECIES(Sloth, Sloth);
ZT_SPECIALIZE_SPECIES(Elephant, Elephant);

#undef ZT_SPECIALIZE_SPECIES

template <class T>
struct CreateAnimalD {
  std::vector<std::unique_ptr<T>> operator()(unsigned qty, unsigned age) {
    std::vector<std::unique_ptr<T>> ts;
    ts.reserve(qty);
    for (decltype(ts.size()) i = 0; i != qty; ++i)
      ts.push_back(make_unique<T>(T(age)));
    return ts;
  }
};

template <class T>
struct CreateAnimalS {
  std::vector<T> operator()(unsigned qty, unsigned age) {
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

std::vector<AnimalSpecies> AllSpecies();
std::string AnimalSpeciesToString(AnimalSpecies s);
std::unique_ptr<Animal> CreateFromSpecies(AnimalSpecies s, unsigned age);


#endif //ZOO_TYCOON_ANIMALSPECIES_H
