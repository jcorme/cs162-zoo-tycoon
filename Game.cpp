#include "Game.h"
#include "GameTurn.h"

void Game::Run() {
  for (;;) {
    GameTurnResult result = GameTurn(player_, base_food_cost_).Run();

    switch (result) {
      case GameTurnResult::Quit:
        std::cout << "Thanks for playing!" << std::endl;
        return;

      case GameTurnResult::PlayerBankrupt:
        std::cout << "GAME OVER: Your zoo has gone bankrupt!" << std::endl;
        return;

      default:
        std::cout << "\nHit enter to continue to the next day...";
        std::cin.ignore();
        std::cout << "\n\n\n==============================\n\n\n" << std::endl;
        SetNewBaseFoodCost();
        break;
    }
  }
}

void Game::SetNewBaseFoodCost() {
  std::uniform_int_distribution<unsigned> uni(
      BASE_FOOD_COST_MIN_PCT_CHANGE, BASE_FOOD_COST_MAX_PCT_CHANGE);
  unsigned pct_change = uni(rng_engine_);
  double dec = static_cast<double>(pct_change) / 100;
  base_food_cost_ = base_food_cost_ * dec;
}