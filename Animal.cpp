#include <iostream>
#include "Animal.h"

Animal::Animal(
    const std::string &name,
    unsigned age,
    unsigned cost,
    unsigned babies_per_birth,
    unsigned food_cost_multiplier,
    double revenue_pct):
    name_(name), age_(age), babies_per_birth_(babies_per_birth), cost_(cost),
    food_cost_multiplier_(food_cost_multiplier), revenue_pct_(revenue_pct) {}

double Animal::DailyRevenue(Option<unsigned> bonus_revenue) const {
  double revenue = revenue_pct_ * cost_;

  if (IsBaby())
    revenue *= 2;

  return revenue;
}

double Animal::FoodCost(FoodType t, double base_cost) const {
  double cost = food_cost_multiplier_ * base_cost;

  if (t == FoodType::Premium) cost *= 2;
  else if (t == FoodType::Cheap) cost /= 2;

  return cost;
}

std::string Animal::PrettyAge() const {
  if (age_ < 365) return std::to_string(age_) + " days";
  if (age_ % 365 == 0) return std::to_string(age_ / 365) + " years";
  unsigned years = age_ / 365;
  unsigned days = age_ - (365 * years);
  return std::to_string(years) + " years and " + std::to_string(days) +" days";
}

bool operator==(const Animal &lhs, const Animal &rhs) {
  return (lhs.name_ == rhs.name_) && (lhs.age_ == rhs.age_);
}
