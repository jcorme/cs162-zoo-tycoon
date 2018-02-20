#ifndef ZOO_TYCOON_PLAYER_H
#define ZOO_TYCOON_PLAYER_H
/*********************************************************************
** Program Filename: Player.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the Player class and its related members. 
** Input: None
** Output: None
*********************************************************************/


#include "Zoo.h"
#include "BankAccount.h"
#include "SpecialEvent.h"
#include "AnimalSpecies.h"
#include "PlayerAction.h"

static constexpr double PLAYER_STARTING_BALANCE = 1e5;

class Player {
  public:
    Player():
        bank_account_(BankAccount(PLAYER_STARTING_BALANCE)), zoo_(Zoo()) {}

    Zoo &zoo() { return zoo_; }

    bool CanAfford(double amount) const {
      return bank_account_.CanAfford(amount); };
    double MoneyRemaining() const { return bank_account_.balance(); }

    void AddMoney(double amount, const std::string &desc) {
      bank_account_.Deposit(amount, desc); };
    std::pair<bool, Option<CAnimalRef>>
        BuyAnimal(AnimalSpecies s, bool adult = true);
    std::pair<bool, Option<std::vector<CAnimalRef>>>
        BuyAnimals(AnimalSpecies s, unsigned qty, bool adults = true);
    bool CareForSickAnimal(const Animal &animal);
    bool FeedAnimal(const Animal &animal, FoodType t, double base_food_cost);
    bool FeedAnimals(FoodType t, double base_cost);
    bool SpendMoney(double amount, const std::string &desc);

    void PrintBankAccountInformation() const { std::cout << bank_account_; }

  private:
    BankAccount bank_account_;

    Zoo zoo_;
};


#endif //ZOO_TYCOON_PLAYER_H