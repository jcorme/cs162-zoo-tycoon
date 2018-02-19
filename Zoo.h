#ifndef ZOO_TYCOON_ZOO_H
#define ZOO_TYCOON_ZOO_H


#include <utility>
#include <vector>
#include "Animal.h"
#include "Option.h"

class Zoo {
  friend std::ostream &operator<<(std::ostream &os, const Zoo &zoo);

  public:
    Zoo() {}

    std::unordered_map<std::string, std::pair<unsigned, unsigned>>
        AdultsAndBabiesForEachSpecies() const;
    std::vector<CAnimalRef> AdultAnimals() const;
    std::vector<CAnimalRef> Animals() const;
    AnimalsVec::size_type NumberOfAnimals() const
        { return animals_.size(); };
    AnimalsVec::size_type NumberOfAdultAnimals() const;
    AnimalsVec::size_type NumberOfBabyAnimals() const;

    void AddAnimal(std::unique_ptr<Animal> animal);
    std::vector<CAnimalRef> AnimalGiveBirth(const Animal &animal);
    void IncrementAnimalAges(unsigned by = 1);
    bool RemoveAnimal(const Animal &animal);

    double FeedingCost(FoodType t, double base_cost) const;
    double TotalDailyRevenue(Option<unsigned> bonus_revenue) const;

  private:
    std::vector<std::unique_ptr<Animal>> animals_;
};

std::ostream &operator<<(std::ostream &os, const Zoo &zoo);


#endif //ZOO_TYCOON_ZOO_H
