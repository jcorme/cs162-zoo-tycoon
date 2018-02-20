#include <utility>
#include "Animal.h"
#include "Player.h"
/*********************************************************************
** Program Filename: Player.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the Player class and in the
 * Player header.
** Input: None
** Output: None
*********************************************************************/

/*********************************************************************
** Function: BuyAnimal
** Description: Purchases an animal of the given species, returning
 * whether the purchase succeeded and an optional reference to the new
 * animal.
** Parameters: s is the species to purchase; adult is whether the new animal
 * should be an adult.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::pair<bool, Option<CAnimalRef>>
Player::BuyAnimal(AnimalSpecies s, bool adult) {
  std::unique_ptr<Animal> animal = CreateFromSpecies(s, adult ? 365 * 3 : 0);
  std::string desc = "Purchased a " + animal->name();
  if (!bank_account_.Withdraw(animal->cost(), desc))
    return std::make_pair(false, None);

  CAnimalRef animal_ref = std::cref(*animal);
  zoo_.AddAnimal(std::move(animal));

  return std::make_pair(true, Option<CAnimalRef>(animal_ref));
}

/*********************************************************************
** Function: BuyAnimals
** Description: Like BuyAnimal, but buys qty animals instead of just one.
** Parameters: s is the species to purchase; qty is the number to purchase;
 * adult is whether the new animals should be adults.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::pair<bool, Option<std::vector<CAnimalRef>>>
Player::BuyAnimals(AnimalSpecies s, unsigned qty, bool adults) {
  if (!CanAfford(CreateFromSpecies(s, 0)->cost() * qty))
    return std::make_pair(false, None);

  std::vector<CAnimalRef> animals;
  animals.reserve(qty);
  for (unsigned i = 0; i != qty; ++i)
    animals.push_back(BuyAnimal(s, adults).second.Unwrap());

  return std::make_pair(true, animals);
}

/*********************************************************************
** Function: CareForSickAnimal
** Description: Treats the given sick animal.
** Parameters: animal is a reference to the sick animal.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool Player::CareForSickAnimal(const Animal &animal) {
  double care_cost = animal.SickCareCost();
  std::string desc = "Care for sick " + animal.name();
  return SpendMoney(care_cost, desc);
}

/*********************************************************************
** Function: FeedAnimal
** Description: Feeds the given animal.
** Parameters: animal is a reference to the animal being fed; t is the
 * type of food they should be fed; base_food_cost is the base cost of
 * food.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool Player::FeedAnimal(
    const Animal &animal, FoodType t, double base_food_cost) {
  double cost = animal.FoodCost(t, base_food_cost);
  std::string desc = "Fed a " + animal.name();
  return SpendMoney(cost, desc);
}

/*********************************************************************
** Function: FeedAnimals
** Description: Like FeedAnimal, but feeds all animals the player's zoo.
** Parameters: t is the type of food they should be fed; base_food_cost
 * is the base cost of food.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool Player::FeedAnimals(FoodType t, double base_cost) {
  if (!CanAfford(zoo_.FeedingCost(t, base_cost))) return false;
  for (const auto &a : zoo_.Animals()) FeedAnimal(a, t, base_cost);
  return true;
}

/*********************************************************************
** Function: SpendMoney
** Description: Withdraws amount of money from the player's bank account,
 * provided they can afford it.
** Parameters: amount is the money to withdraw; desc is the reason for the
 * withdrawal.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool Player::SpendMoney(double amount, const std::string &desc) {
  if (!CanAfford(amount)) return false;
  bank_account_.Withdraw(amount, desc);
  return true;
}

