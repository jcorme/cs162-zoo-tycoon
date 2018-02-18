#include <functional>
#include <vector>
#include "Option.h"
#include "Zoo.h"

void Zoo::AddAnimal(std::unique_ptr<Animal> animal)
{
  animals_.push_back(std::move(animal));
}

bool Zoo::RemoveAnimal(const Animal &animal)
{
  auto it = std::find_if(animals_.begin(), animals_.end(),
      [&](const std::unique_ptr<Animal> &a) {
          return *a == animal;
      });
  if (it == animals_.end()) return false;
  animals_.erase(it);
  return true;
}

std::vector<CAnimalRef> Zoo::AnimalGiveBirth(const Animal &animal)
{
  AnimalsVec babies = animal.GiveBirth();
  std::vector<CAnimalRef> birthed_animals;
  for (const auto &b : babies)
    birthed_animals.push_back(std::cref(*b));
  std::move(babies.begin(), babies.end(), std::back_inserter(animals_));
  return birthed_animals;
}

std::vector<CAnimalRef> Zoo::Animals() const
{
  std::vector<CAnimalRef> animals;
  animals.reserve(animals_.size());
  for (const auto &a : animals_)
    animals.push_back(std::cref(*a));
  return animals;
}

std::vector<CAnimalRef> Zoo::AdultAnimals() const
{
  std::vector<CAnimalRef> adult_animals;
  for (const auto &a : animals_)
    if (a->IsAdult())
      adult_animals.push_back(std::cref(*a));
  return adult_animals;
}

AnimalsVec::size_type Zoo::NumberOfAdultAnimals() const
{
  AnimalsVec::size_type n = 0;
  for (const auto &a : animals_)
    if (a->IsAdult()) ++n;
  return n;
}

AnimalsVec::size_type Zoo::NumberOfBabyAnimals() const
{
  AnimalsVec::size_type n = 0;
  for (const auto &a : animals_)
    if (a->IsBaby()) ++n;
  return n;
}

std::unordered_map<std::string, std::pair<unsigned, unsigned>>
Zoo::AdultsAndBabiesForEachSpecies() const
{
  std::unordered_map<std::string, std::pair<unsigned, unsigned>> map;
  for (const auto &a : animals_)
  {
    if (map.find(a->name()) == map.end())
      map[a->name()] = std::make_pair(0, 0);
    if (a->IsAdult()) ++(map[a->name()].first);
    else if (a->IsBaby()) ++(map[a->name()].second);
  }

  return map;
};

double Zoo::FeedingCost(FoodType t, double base_cost) const
{
  unsigned cost = 0;
  for (const auto &a : animals_)
    cost += a->FoodCost(t, base_cost);
  return cost;
}

double Zoo::TotalDailyRevenue(Option<unsigned> bonus_revenue) const
{
  double revenue = 0.0;
  for (const auto &a : animals_)
    revenue += a->DailyRevenue(bonus_revenue);
  return revenue;
}

void Zoo::IncrementAnimalAges(unsigned int by)
{
  for (auto &a : animals_)
    a->IncrementAge(by);
}

std::ostream &operator<<(std::ostream &os, const Zoo &zoo)
{
  os << "Animals in your zoo:\n";
  auto sz = zoo.animals_.size();
  for (decltype(sz) i = 0; i != sz; ++i)
  {
    const Animal *animal = zoo.animals_[i].get();
    os << '\t' << animal->name() << ": " << animal->PrettyAge() << " old";
    if (i != sz - 1) os << '\n';
  }

  return os;
}
