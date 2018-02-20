#ifndef ZOO_TYCOON_ANIMAL_H
#define ZOO_TYCOON_ANIMAL_H
/*********************************************************************
** Program Filename: Animal.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the Animal class and its related members. 
** Input: None
** Output: None
*********************************************************************/


#include <vector>
#include "Option.h"
#include "Utils.h"
#include "FoodType.h"

class Animal;

using AnimalsVec = std::vector<std::unique_ptr<Animal>>;
using CAnimalRef = std::reference_wrapper<const Animal>;

class Animal {
  friend bool operator==(const Animal &lhs, const Animal &rhs);

  public:
    Animal(
        const std::string &name,
        unsigned age,
        unsigned cost,
        unsigned babies_per_birth,
        unsigned food_cost_multiplier,
        double revenue_pct = 0.05);

    unsigned age() const { return age_; }
    unsigned babies_per_birth() const { return babies_per_birth_; }
    unsigned cost() const { return cost_; }
    const std::string &name() const { return name_; }

    virtual double DailyRevenue(Option<unsigned> bonus_revenue) const;
    double FoodCost(FoodType t, double base_cost) const;
    bool IsBaby() const { return age_ < 30; }
    inline bool IsAdult() const;
    std::string PrettyAge() const;
    double SickCareCost() const { return static_cast<double>(cost_) / 2; }

    void IncrementAge(unsigned by = 1) { age_ += by; }

    virtual AnimalsVec GiveBirth() const = 0;

  private:
    // Name (right now) is just the species name.
    std::string name_;

    unsigned age_;
    unsigned babies_per_birth_;
    unsigned cost_;
    unsigned food_cost_multiplier_;
    // Animals generate revenue equal to percentage of the cost of
    // one of their species (default = 5%).
    double revenue_pct_;
};

bool operator==(const Animal &lhs, const Animal &rhs);

/*********************************************************************
** Function: IsAdult
** Description: Returns whether the animal is an adult.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool Animal::IsAdult() const {
  return (static_cast<double>(age_) / 365) >= 3;
}

/*********************************************************************
** Function: AnimalGiveBirth
** Description: Template function that dynamically allocates (possibly
 * more than one) Animal subclass.
** Parameters: animal is the parent animal giving birth.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
AnimalsVec AnimalGiveBirth(const Animal *animal) {
  AnimalsVec babies;
  babies.reserve(animal->babies_per_birth());
  for (decltype(babies.size()) i = 0; i != animal->babies_per_birth(); ++i)
    babies.push_back(make_unique<T>(T()));
  return babies;
}


#endif //ZOO_TYCOON_ANIMAL_H
