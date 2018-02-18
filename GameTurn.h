#ifndef ZOO_TYCOON_GAMETURN_H
#define ZOO_TYCOON_GAMETURN_H


#include <map>
#include <string>
#include "Option.h"
#include "AnimalSpecies.h"
#include "SpecialEvent.h"
#include "Player.h"
#include "PlayerAction.h"

static constexpr unsigned MAX_ANIMAL_PURCHASES = 2;

enum class GameTurnResult
{
    Continue,
    InsufficientFunds,
    MissingAnimals,
    PlayerBankrupt,
    Quit
};

class GameTurn
{
  friend class Game;

  public:
    GameTurnResult Run();

  private:
    GameTurn(Player &player, double base_food_cost);

    static unsigned day_;

    Option<std::pair<AnimalSpecies, unsigned>> animals_bought_ = None;
    Player &player_;
    Zoo &zoo_;

    static FoodType food_type_;
    SpecialEvent special_event_;

    double base_food_cost_;
    Option<unsigned> monkey_bonus_revenue_;

    void IncrementAnimalAges();
    Option<GameTurnResult> HandleSpecialEvent();
    Option<GameTurnResult> HandleMainAction(PlayerMainAction action);
    Option<GameTurnResult> AnimalBirth(Option<CAnimalRef> parent);
    Option<GameTurnResult> SickAnimal(Option<CAnimalRef> sick_animal);
    PlayerMainAction PromptPlayerMainMenu();
    Option<std::pair<AnimalSpecies, unsigned>> PromptPlayerBuyAnimal();
    Option<unsigned> PromptPlayerPurchaseQuantity(AnimalSpecies s) const;

    bool PlayerBuyAnimal(AnimalSpecies s, unsigned qty);
    void GivePlayerRevenue();
    void PrintGameState() const;
    Option<GameTurnResult> FeedAnimals();

    bool CanBuyAnimal() const;
    FoodType PromptPlayerFoodType() const;
};


#endif //ZOO_TYCOON_GAMETURN_H
