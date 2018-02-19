#include <utility>
#include "Animal.h"
#include "Player.h"

bool Player::BuyAnimals(AnimalSpecies s, unsigned qty, bool adults)
{
  if (!CanAfford(CreateFromSpecies(s, 0)->cost() * qty)) return false;
  for (unsigned i = 0; i != qty; ++i) BuyAnimal(s, adults);
  return true;
}

bool Player::BuyAnimal(AnimalSpecies s, bool adult)
{
  std::unique_ptr<Animal> animal = CreateFromSpecies(s, adult ? 365 * 3 : 0);
  std::string desc = "Purchased a " + animal->name();
  if (!bank_account_.Withdraw(animal->cost(), desc)) return false;
  zoo_.AddAnimal(std::move(animal));
  return true;
}

bool Player::CareForSickAnimal(const Animal &animal) {
  double care_cost = animal.SickCareCost();
  std::string desc = "Care for sick " + animal.name();
  return SpendMoney(care_cost, desc);
}

bool Player::FeedAnimal(
    const Animal &animal, FoodType t, double base_food_cost) {
  double cost = animal.FoodCost(t, base_food_cost);
  std::string desc = "Fed a " + animal.name();
  return SpendMoney(cost, desc);
}

bool Player::FeedAnimals(FoodType t, double base_cost) {
  if (!CanAfford(zoo_.FeedingCost(t, base_cost))) return false;
  for (const auto &a : zoo_.Animals()) FeedAnimal(a, t, base_cost);
  return true;
}

bool Player::SpendMoney(double amount, const std::string &desc) {
  if (!CanAfford(amount)) return false;
  bank_account_.Withdraw(amount, desc);
  return true;
}

