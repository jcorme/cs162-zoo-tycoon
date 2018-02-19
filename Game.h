#ifndef ZOO_TYCOON_GAME_H
#define ZOO_TYCOON_GAME_H


#include <map>
#include "Zoo.h"
#include "Player.h"

static constexpr unsigned DEFAULT_BASE_FOOD_COST = 50;
static constexpr unsigned BASE_FOOD_COST_MIN_PCT_CHANGE = 75;
static constexpr unsigned BASE_FOOD_COST_MAX_PCT_CHANGE = 125;

class Game {
  public:
    explicit Game(Player &&player):
        player_(std::move(player)), zoo_(player_.zoo()),
        base_food_cost_(DEFAULT_BASE_FOOD_COST) {}
    Game(): Game(Player()) {}

    void Run();

  private:
    std::mt19937 rng_engine_ = MakeRngEngine();

    Player player_;

    Zoo &zoo_;

    double base_food_cost_;

    void SetNewBaseFoodCost();
};


#endif //ZOO_TYCOON_GAME_H
