#ifndef ZOO_TYCOON_PLAYERACTION_H
#define ZOO_TYCOON_PLAYERACTION_H


#include <unordered_map>
#include "AnimalSpecies.h"
#include "FoodType.h"

enum class PlayerMainAction
{
  BuyAnimal,
  ViewZooAnimals,
  CheckBank,
  PrintGameState,
  EndTurn,
  QuitGame
};

template <class T>
struct ActionString;

template <>
struct ActionString<PlayerMainAction>
{
  static const ActionStringMap<PlayerMainAction> Strings;
};

template <>
struct ActionString<AnimalSpecies>
{
  static const ActionStringMap<AnimalSpecies> Strings;
};

template <>
struct ActionString<FoodType>
{
  static const ActionStringMap<FoodType> Strings;
};

template <>
struct ActionString<unsigned>
{
  static const ActionStringMap<unsigned> Strings;
};

std::vector<PlayerMainAction> AllMainActions();
std::vector<FoodType> AllFoodOptions();


#endif //ZOO_TYCOON_PLAYERACTION_H
