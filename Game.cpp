/*********************************************************************
** Program Filename: Game.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared in the Game class and in
 * the Game header.
** Input: None
** Output: None
*********************************************************************/
#include "Game.h"
#include "GameTurn.h"

/*********************************************************************
** Function: Run
** Description: Initiates the game loop, continuing until the player is
 * bankrupt.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
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

/*********************************************************************
** Function: SetNewBaseFoodCost
** Description: Sets the base food cost to 75-125% of its current value.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
void Game::SetNewBaseFoodCost() {
  std::uniform_int_distribution<unsigned> uni(
      BASE_FOOD_COST_MIN_PCT_CHANGE, BASE_FOOD_COST_MAX_PCT_CHANGE);
  unsigned pct_change = uni(rng_engine_);
  double dec = static_cast<double>(pct_change) / 100;
  base_food_cost_ = base_food_cost_ * dec;
}