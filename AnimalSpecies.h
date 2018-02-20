#ifndef ZOO_TYCOON_ANIMALSPECIES_H
#define ZOO_TYCOON_ANIMALSPECIES_H
/*********************************************************************
** Program Filename: AnimalSpecies.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the AnimalSpecies enum and related templates. 
** Input: None
** Output: None
*********************************************************************/


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

// SpeciesToType is used to map AnimalSpecies values to their corresponding
// Animal subclass type, since we can't partially specialize template aliases.
// Example of usage: SpeciesToType<AnimalSpecies::Monkey>::type
// The above example gives the type Monkey.
template <AnimalSpecies A>
struct SpeciesToType;

// Makes the specialization for each species as simple as possible.
// Undefined right after usage.
#define ZT_SPECIALIZE_SPECIES(A, T) \
template <> struct SpeciesToType<AnimalSpecies::A> { using type = T; };

ZT_SPECIALIZE_SPECIES(Monkey, Monkey);
ZT_SPECIALIZE_SPECIES(SeaOtter, SeaOtter);
ZT_SPECIALIZE_SPECIES(Sloth, Sloth);
ZT_SPECIALIZE_SPECIES(Elephant, Elephant);

#undef ZT_SPECIALIZE_SPECIES

template <class T>
struct CreateAnimalD {
  /*********************************************************************
  ** Function: operator()
  ** Description: Overloads the function call operator; returns a vector of
   * dynamically allocated Animal objects of Animal subclass type.
  ** Parameters: qty is the number of Animals to create; age is the age they
   * should all have.
  ** Pre-Conditions: None
  ** Post-Conditions: None
  *********************************************************************/
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
  /*********************************************************************
  ** Function: operator()
  ** Description: Overloads the function call operator; returns a vector
   * normal Animal objects of Animal subclass type.
  ** Parameters: qty is the number of Animals to create; age is the age they
   * should all have.
  ** Pre-Conditions: None
  ** Post-Conditions: None
  *********************************************************************/
  std::vector<T> operator()(unsigned qty, unsigned age) {
    std::vector<T> ts;
    ts.reserve(qty);
    for (auto i = 0; i != qty; ++i)
      ts.push_back(T(age));
    return ts;
  }
};

// The two following aliases exist for convenience, avoiding the need for
// those long type signatures.
template <AnimalSpecies A>
using CreateDynamicAnimal = CreateAnimalD<typename SpeciesToType<A>::type>;

template <AnimalSpecies A>
using CreateStaticAnimal = CreateAnimalS<typename SpeciesToType<A>::type>;

extern const ActionStringMap<AnimalSpecies> AnimalSpeciesToStringMap;

std::vector<AnimalSpecies> AllSpecies();
std::string AnimalSpeciesToString(AnimalSpecies s);
std::unique_ptr<Animal> CreateFromSpecies(AnimalSpecies s, unsigned age);


#endif //ZOO_TYCOON_ANIMALSPECIES_H
