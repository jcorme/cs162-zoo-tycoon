#ifndef ZOO_TYCOON_ZOO_H
#define ZOO_TYCOON_ZOO_H


#include <utility>
#include <vector>
#include "Animal.h"
#include "Option.h"

class Zoo
{
  friend std::ostream &operator<<(std::ostream &os, const Zoo &zoo);

  public:
    Zoo() {}

    void AddAnimal(std::unique_ptr<Animal> animal);
    bool RemoveAnimal(const Animal &animal);
    std::vector<CAnimalRef> AnimalGiveBirth(const Animal &animal);

    std::vector<std::reference_wrapper<const Animal>> Animals() const;
    std::vector<std::reference_wrapper<const Animal>> AdultAnimals() const;
    AnimalsVec::size_type NumberOfAnimals() const
        { return animals_.size(); };
    AnimalsVec::size_type NumberOfAdultAnimals() const;
    AnimalsVec::size_type NumberOfBabyAnimals() const;
    std::unordered_map<std::string, std::pair<unsigned, unsigned>>
        AdultsAndBabiesForEachSpecies() const;

    double FeedingCost(FoodType t, double base_cost) const;
    double TotalDailyRevenue(Option<unsigned> bonus_revenue) const;

    void IncrementAnimalAges(unsigned by = 1);

    Option<const Animal *> operator[](AnimalsVec::size_type idx) const;

  private:
    std::vector<std::unique_ptr<Animal>> animals_;
};

std::ostream &operator<<(std::ostream &os, const Zoo &zoo);


#endif //ZOO_TYCOON_ZOO_H
