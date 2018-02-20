/*********************************************************************
** Program Filename: Zoo.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the Zoo class and in the
 * Zoo header.
** Input: None
** Output: None
*********************************************************************/
#include <functional>
#include <vector>
#include "Option.h"
#include "Zoo.h"

/*********************************************************************
** Function: AdultsAndBabiesForEachSpecies
** Description: Returns a map containing the names of every species in the
 * zoo mapped to a pair containing the number of adults and babies of that
 * species (in that order).
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::unordered_map<std::string, std::pair<unsigned, unsigned>>
Zoo::AdultsAndBabiesForEachSpecies() const {
  std::unordered_map<std::string, std::pair<unsigned, unsigned>> map;
  for (const auto &a : animals_) {
    if (map.find(a->name()) == map.end())
      map[a->name()] = std::make_pair(0, 0);
    if (a->IsAdult()) ++(map[a->name()].first);
    else if (a->IsBaby()) ++(map[a->name()].second);
  }

  return map;
}

/*********************************************************************
** Function: AdultAnimals
** Description: Returns a vector of references to all adult animals in the
 * zoo.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::vector<CAnimalRef> Zoo::AdultAnimals() const {
  std::vector<CAnimalRef> adult_animals;
  for (const auto &a : animals_)
    if (a->IsAdult())
      adult_animals.push_back(std::cref(*a));
  return adult_animals;
}

/*********************************************************************
** Function: Animals
** Description: Returns a vector of references to all animals in the zoo.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::vector<CAnimalRef> Zoo::Animals() const {
  std::vector<CAnimalRef> animals;
  animals.reserve(animals_.size());
  for (const auto &a : animals_)
    animals.push_back(std::cref(*a));
  return animals;
}

/*********************************************************************
** Function: NumberOfAdultAnimals
** Description: Counts all the adult animals in the zoo.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
AnimalsVec::size_type Zoo::NumberOfAdultAnimals() const {
  AnimalsVec::size_type n = 0;
  for (const auto &a : animals_)
    if (a->IsAdult()) ++n;
  return n;
}

/*********************************************************************
** Function: NumberOfBabyAnimals
** Description: Counts all the baby animals in the zoo.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: none
*********************************************************************/
AnimalsVec::size_type Zoo::NumberOfBabyAnimals() const {
  AnimalsVec::size_type n = 0;
  for (const auto &a : animals_)
    if (a->IsBaby()) ++n;
  return n;
}

/*********************************************************************
** Function: AddAnimal
** Description: Adds the given animal to the zoo.
** Parameters: animal is the Animal pointer to add.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
void Zoo::AddAnimal(std::unique_ptr<Animal> animal) {
  animals_.push_back(std::move(animal));
}

/*********************************************************************
** Function: AnimalGiveBirth
** Description: Adds the babies of the given animal to the zoo; the given
 * animal gives birth to babies_per_birth() number of babies.
** Parameters: animal is the parent to give birth.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::vector<CAnimalRef> Zoo::AnimalGiveBirth(const Animal &animal) {
  AnimalsVec babies = animal.GiveBirth();
  std::vector<CAnimalRef> birthed_animals;
  for (const auto &b : babies)
    birthed_animals.push_back(std::cref(*b));
  std::move(babies.begin(), babies.end(), std::back_inserter(animals_));
  return birthed_animals;
}

/*********************************************************************
** Function: IncrementAnimalAges
** Description: Increments the ages of every zoo animal by by amount.
** Parameters: by is the amount to increase their ages.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
void Zoo::IncrementAnimalAges(unsigned int by) {
  for (auto &a : animals_)
    a->IncrementAge(by);
}

/*********************************************************************
** Function: RemoveAnimal
** Description: Removes the given animal from the zoo.
** Parameters: animal is the Animal object to remove.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool Zoo::RemoveAnimal(const Animal &animal) {
  auto it = std::find_if(animals_.begin(), animals_.end(),
      [&](const std::unique_ptr<Animal> &a) {
          return *a == animal;
      });
  if (it == animals_.end()) return false;
  animals_.erase(it);
  return true;
}

/*********************************************************************
** Function: FeedingCost
** Description: Returns the cost of feeding every animal in the zoo.
** Parameters: t is the type of feed being fed to the animals; base_cost
 * is the base cost of the feed.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
double Zoo::FeedingCost(FoodType t, double base_cost) const {
  unsigned cost = 0;
  for (const auto &a : animals_)
    cost += a->FoodCost(t, base_cost);
  return cost;
}

/*********************************************************************
** Function: TotalDailyRevenue
** Description: Calculates the total daily revenue generated by the zoo.
** Parameters: bonus_revenue is an optional amount of bonus revenue for
 * each animal (currently only applies to monkeys).
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
double Zoo::TotalDailyRevenue(Option<unsigned> bonus_revenue) const {
  double revenue = 0.0;
  for (const auto &a : animals_)
    revenue += a->DailyRevenue(bonus_revenue);
  return revenue;
}

/*********************************************************************
** Function: operator<<
** Description: Overloads the insertion operator to print Zoo objects.
** Parameters: os is the output stream; zoo is the Zoo to print.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::ostream &operator<<(std::ostream &os, const Zoo &zoo) {
  os << "Animals in your zoo:\n";
  auto sz = zoo.animals_.size();
  for (decltype(sz) i = 0; i != sz; ++i) {
    const Animal *animal = zoo.animals_[i].get();
    os << '\t' << animal->name() << ": " << animal->PrettyAge() << " old";
    if (i != sz - 1) os << '\n';
  }

  return os;
}
