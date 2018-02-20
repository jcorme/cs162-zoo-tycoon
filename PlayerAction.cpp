/*********************************************************************
** Program Filename: PlayerAction.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions and members declared in the PlayerAction
 * header.
** Input: None
** Output: None
*********************************************************************/
#include "PlayerAction.h"

// Maps PlayerMainAction values to a string.
const ActionStringMap<PlayerMainAction>
ActionString<PlayerMainAction>::Strings = {
  { PlayerMainAction::BuyAnimal, "Buy an animal." },
  { PlayerMainAction::ViewZooAnimals, "List the animals in your zoo." },
  { PlayerMainAction::CheckBank, "Check your bank account." },
  { PlayerMainAction::PrintGameState,
      "Print the current state of the game." },
  { PlayerMainAction::EndTurn, "End the current turn." },
  { PlayerMainAction::QuitGame, "Quit game." }
};

// Maps AnimalSpecies values to a string.
const ActionStringMap<AnimalSpecies>
ActionString<AnimalSpecies>::Strings = {
  { AnimalSpecies::Monkey, "Purchase a monkey." },
  { AnimalSpecies::SeaOtter, "Purchase a sea otter." },
  { AnimalSpecies::Sloth, "Purchase a sloth."},
  { AnimalSpecies::Elephant, "Purchase an elephant."}
};

// Maps FoodType values to a string.
const ActionStringMap<FoodType> ActionString<FoodType>::Strings = {
  { FoodType::Regular, "Feed your animals normal food." },
  { FoodType::Premium, "Feed your animals premium food; "
      "twice the cost, but reduces chance of sickness by two." },
  { FoodType::Cheap, "Feed your animals cheap food; "
      "half the cost, but increases chance of sickness by two." }
};

// Little trick to please the compiler.
const ActionStringMap<unsigned> ActionString<unsigned>::Strings = {};

/*********************************************************************
** Function: AllFoodOptions
** Description: Returns a vector of all possible FoodType values.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::vector<FoodType> AllFoodOptions() {
  return ActionStringMapKeys(ActionString<FoodType>::Strings);
}

/*********************************************************************
** Function: AllMainActions
** Description: Returns a vector of all PlayerMainAction values.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::vector<PlayerMainAction> AllMainActions() {
  return ActionStringMapKeys(ActionString<PlayerMainAction>::Strings);
}

