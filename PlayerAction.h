#ifndef ZOO_TYCOON_PLAYERACTION_H
#define ZOO_TYCOON_PLAYERACTION_H


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

template <class T>
struct ActionString;

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
