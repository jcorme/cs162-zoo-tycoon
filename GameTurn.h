#ifndef ZOO_TYCOON_GAMETURN_H
#define ZOO_TYCOON_GAMETURN_H
/*********************************************************************
** Program Filename: GameTurn.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the GameTurn class and its related members. 
** Input: None
** Output: None
*********************************************************************/


#include <map>
#include <string>
#include "Option.h"
#include "AnimalSpecies.h"
#include "SpecialEvent.h"
#include "Player.h"
#include "PlayerAction.h"

static constexpr unsigned MAX_ANIMAL_PURCHASES = 2;

enum class GameTurnResult {
  Continue,
  InsufficientFunds,
  PlayerBankrupt,
  Quit
};

class GameTurn {
  friend class Game;

  public:
    GameTurnResult Run();

    void PrintGameState() const;

  private:
    using AnimalPurchase = std::pair<AnimalSpecies, unsigned>;

    GameTurn(Player &player, double base_food_cost);

    // The current day of the game.
    static unsigned day_;
    // The type of food to feed the animals this turn.
    static FoodType food_type_;

    // Keeps track of what, if any, and how many animals the player has
    // purchased this turn.
    Option<AnimalPurchase> animals_bought_ = None;
    Player &player_;
    Zoo &zoo_;

    SpecialEvent special_event_;

    double base_food_cost_;
    Option<unsigned> monkey_bonus_revenue_;

    Option<GameTurnResult> AnimalBirth(CAnimalRef parent);
    Option<GameTurnResult> FeedAnimals();
    void GivePlayerRevenue();
    Option<GameTurnResult> HandleSpecialEvent();
    Option<GameTurnResult> HandleMainAction(PlayerMainAction action);
    Option<GameTurnResult> PlayerBuyAnimal(AnimalSpecies s, unsigned qty);
    Option<GameTurnResult> SickAnimal(CAnimalRef sick_animal);

    Option<AnimalPurchase> BuildAnimalPurchase(AnimalSpecies s) const;
    Option<AnimalPurchase> PromptPlayerBuyAnimal();
    FoodType PromptPlayerFoodType() const;
    PlayerMainAction PromptPlayerMainMenu();

    bool CanBuyAnimal() const;
};


#endif //ZOO_TYCOON_GAMETURN_H
