#ifndef ZOO_TYCOON_PLAYERACTION_H
#define ZOO_TYCOON_PLAYERACTION_H
/*********************************************************************
** Program Filename: PlayerAction.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the ActionString template, the PlayerMainAction
 * enum, and related functions; all having to do with user prompts.
** Input: None
** Output: None
*********************************************************************/


#include <unordered_map>
#include "AnimalSpecies.h"
#include "FoodType.h"

enum class PlayerMainAction {
  BuyAnimal,
  ViewZooAnimals,
  CheckBank,
  PrintGameState,
  EndTurn,
  QuitGame
};

// Specializations map T values to strings presentable to the user.
template <class T>
struct ActionString;

// Makes ActionString specialization simpler.
// Undefined right after usage.
#define ZT_SPECIALIZE_ACTION_STRING(T) \
template <> struct ActionString<T> { static const ActionStringMap<T> Strings; };

ZT_SPECIALIZE_ACTION_STRING(PlayerMainAction);
ZT_SPECIALIZE_ACTION_STRING(AnimalSpecies);
ZT_SPECIALIZE_ACTION_STRING(FoodType);
ZT_SPECIALIZE_ACTION_STRING(unsigned);

#undef ZT_SPECIALIZE_ACTION_STRING

std::vector<FoodType> AllFoodOptions();
std::vector<PlayerMainAction> AllMainActions();


#endif //ZOO_TYCOON_PLAYERACTION_H
