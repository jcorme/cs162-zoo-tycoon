#include <iostream>
#include "GameTurn.h"
#include "MenuPrompt.h"

unsigned GameTurn::day_ = 0;
FoodType GameTurn::food_type_ = FoodType::Regular;

GameTurn::GameTurn(Player &player, double base_food_cost):
    player_(player), zoo_(player.zoo()),
    special_event_(SpecialEvent(player.zoo(), food_type_)),
    base_food_cost_(base_food_cost), monkey_bonus_revenue_(None) {
  food_type_= PromptPlayerFoodType();
  std::cout << "\n\n" << std::endl;
  ++day_;
}

GameTurnResult GameTurn::Run() {
  zoo_.IncrementAnimalAges();
  PrintGameState();
  FeedAnimals();

  GameTurnResult handle_result = HandleSpecialEvent()
      .UnwrapOr(GameTurnResult::Continue);
  if (handle_result == GameTurnResult::PlayerBankrupt) return handle_result;

  for (;;) {
    PlayerMainAction action = PromptPlayerMainMenu();

    if (action == PlayerMainAction::EndTurn) break;
    else if (action == PlayerMainAction::QuitGame) return GameTurnResult::Quit;
    else {
      GameTurnResult result = HandleMainAction(action)
          .UnwrapOr(GameTurnResult::Continue);

      switch (result) {
        case GameTurnResult::InsufficientFunds:
          std::cout << "\nUnable to make purchase due to insufficient funds.\n";
          break;

        case GameTurnResult::PlayerBankrupt:
          return result;

        default: break;
      }
    }
  }

  GivePlayerRevenue();

  return GameTurnResult::Continue;
}

void GameTurn::PrintGameState() const {
  using Map = std::unordered_map<std::string, std::pair<unsigned, unsigned>>;

  std::cout << "Day " << day_ << " -- CURRENT STATE OF THE GAME: " << '\n'
            << "\tBank Account Balance: " << player_.MoneyRemaining() << '\n'
            << "\t# of Adult Animals: " << zoo_.NumberOfAdultAnimals() << '\n'
            << "\t# of Baby Animals: " << zoo_.NumberOfBabyAnimals() << '\n'
            << "\t# of Adults/Babies of Each Species:\n";

  Map counts = zoo_.AdultsAndBabiesForEachSpecies();
  for (const auto &c : counts)
    std::cout << "\t\t" << c.first << ": " << c.second.first << " adults and "
              << c.second.second << " babies." << '\n';

  std::cout << '\n' << std::endl;
}

Option<GameTurnResult> GameTurn::AnimalBirth(CAnimalRef parent) {
  std::cout << "An adult " << parent.get().name() << " gave birth to "
            << parent.get().babies_per_birth() << " babies!\n";

  std::vector<CAnimalRef> babies = zoo_.AnimalGiveBirth(parent);
  for (const auto &b : babies) {
    if (!player_.FeedAnimal(b, food_type_, base_food_cost_)) {
      std::cout << "You don't have enough money to feed your newborn "
                << b.get().name() << "!\n";

      return GameTurnResult::PlayerBankrupt;
    }
  }

  double feeding_cost = parent.get().FoodCost(food_type_, base_food_cost_);
  std::cout << "Successfully fed " << parent.get().babies_per_birth()
            << " newborns; paid $" << feeding_cost << ".\n";

  return None;
}

Option<GameTurnResult> GameTurn::FeedAnimals() {
  if (!player_.FeedAnimals(food_type_, base_food_cost_))
    return GameTurnResult::PlayerBankrupt;

  double feeding_cost = zoo_.FeedingCost(food_type_, base_food_cost_);
  if (feeding_cost > 0)
    std::cout << "Successfully fed all the animals; paid $" << feeding_cost
              << '.' << std::endl;

  return None;
}

void GameTurn::GivePlayerRevenue() {
  std::string n_animals = std::to_string(zoo_.NumberOfAnimals());
  std::string desc = "Daily zoo revenue from " + n_animals + " animals";
  double total_revenue = zoo_.TotalDailyRevenue(monkey_bonus_revenue_);

  player_.AddMoney(total_revenue, desc);

  std::cout << "\nThe zoo made $" << total_revenue << " today, bringing your "
            << "bank balance to $" << player_.MoneyRemaining() << ".\n";
}

Option<GameTurnResult> GameTurn::HandleSpecialEvent() {
  switch (special_event_.type()) {
    case SpecialEventType::AnimalBirth:
      return special_event_.animal_birth().AndThen<GameTurnResult>(
          [&](CAnimalRef parent) { return AnimalBirth(parent); });

    case SpecialEventType::SickAnimal:
      return special_event_.sick_animal().AndThen<GameTurnResult>(
          [&](CAnimalRef animal) { return SickAnimal(animal); });

    case SpecialEventType::ZooAttendanceBoom:
      monkey_bonus_revenue_ = special_event_.monkey_bonus_revenue();
      std::cout << "There is a zoo attendance boom today! Each monkey will "
                << "generate an extra $" << monkey_bonus_revenue_.CUnwrapRef()
                << " in revenue today!\n";
      return None;

    default: return None;
  }
}

Option<GameTurnResult> GameTurn::HandleMainAction(PlayerMainAction action) {
  switch (action) {
    case PlayerMainAction::BuyAnimal:
      return PromptPlayerBuyAnimal().AndThen<GameTurnResult>(
          [&](std::pair<AnimalSpecies, unsigned> purchase) {
              return PlayerBuyAnimal(purchase.first, purchase.second);
          }
      );

    case PlayerMainAction::ViewZooAnimals:
      std::cout << zoo_ << std::endl;
      break;

    case PlayerMainAction::CheckBank:
      std::cout << "Your Bank Account Information: " << "\n\n";
      player_.PrintBankAccountInformation();
      std::cout << '\n' << std::endl;
      break;

    case PlayerMainAction::PrintGameState:
      PrintGameState();
      std::cout << std::endl;
      break;

    default:
      break;
  }

  return None;
}

Option<GameTurnResult>
GameTurn::PlayerBuyAnimal(AnimalSpecies s, unsigned qty) {
  std::pair<bool, Option<std::vector<CAnimalRef>>> result =
      player_.BuyAnimals(s, qty);
  if (!result.first) return GameTurnResult::InsufficientFunds;

  std::cout << "\nYou purchased " << qty << ' ' << AnimalSpeciesToString(s)
            << "s!\n";

  double food_cost = 0;
  for (const auto &i : result.second.CUnwrapRef()) {
    if (!player_.FeedAnimal(i.get(), food_type_, base_food_cost_)) {
      std::cout << "You don't have enough money to feed your newly purchased "
                << i.get().name() << "!\n";
      return GameTurnResult::PlayerBankrupt;
    }

    food_cost = i.get().FoodCost(food_type_, base_food_cost_);
  }

  std::cout << "You paid $" << food_cost * qty << " to feed your "
            << qty << " new " << AnimalSpeciesToString(s) << "s.\n";

  if (animals_bought_.IsSome())
    animals_bought_.UnwrapRef().second += qty;
  else
    animals_bought_ = std::make_pair(s, qty);

  return None;
}

Option<GameTurnResult> GameTurn::SickAnimal(CAnimalRef sick_animal) {
  std::cout << "A " << sick_animal.get().name() << " fell sick!\n";

  if (!player_.CareForSickAnimal(sick_animal)) {
    std::cout << "Since you cannot afford to pay for their medical costs, "
              << "the " << sick_animal.get().name() << " has died.\n";
    zoo_.RemoveAnimal(sick_animal);
  } else {
    std::cout << "You paid $" << sick_animal.get().SickCareCost()
              << " in medical costs to treat the " << sick_animal.get().name()
              << ".\n";
  }

  return None;
}

Option<GameTurn::AnimalPurchase>
GameTurn::BuildAnimalPurchase(AnimalSpecies s) const {
  if (!CanBuyAnimal()) return None;

  std::string animal_type = AnimalSpeciesToString(s);
  std::cout << "\nHow many " << animal_type << "s would you like to buy?\n";

  MenuPrompt<unsigned> prompt(true);
  prompt.AddOptions({1,2});
  ActionStringMap<unsigned> options_map = {
      {1, "One"},
      {2, "Two"}
  };

  if (animals_bought_.IsSome()) prompt.RemoveOption(2);
  prompt.OverrideStrings(options_map);
  return prompt().Map<AnimalPurchase>([s](unsigned qty) {
      return std::make_pair(s, qty);
  });
}

Option<GameTurn::AnimalPurchase> GameTurn::PromptPlayerBuyAnimal() {
  if (!CanBuyAnimal()) return None;

  std::vector<AnimalSpecies> animal_options;
  Option<std::string> prompt_msg = animals_bought_.MapCRef<std::string>(
      [&](const AnimalPurchase &p) {
          // small side effect
          animal_options.push_back(p.first);

          std::string animal_type = AnimalSpeciesToString(p.first);
          std::ostringstream oss;

          oss << "\nYou've already purchased " << p.second << ' '
              << animal_type << " this turn, so the only thing you can buy is "
              << MAX_ANIMAL_PURCHASES - p.second << " more " << animal_type
              << ".\n"
              << "What would you like to do?";

          return oss.str();
      }
  );

  if (animal_options.empty()) animal_options = AllSpecies();

  using ABPair = std::pair<AnimalSpecies, unsigned>;

  MenuPrompt<AnimalSpecies> prompt(true);
  prompt.AddOptions(animal_options);
  return prompt(prompt_msg).AndThen<AnimalPurchase>(
      [&](AnimalSpecies s) { return BuildAnimalPurchase(s); });
}

FoodType GameTurn::PromptPlayerFoodType() const {
  MenuPrompt<FoodType> prompt;
  prompt.AddOptions(AllFoodOptions());
  std::cout << "\nWhat food would you like to feed your animals today?\n";
  return prompt().Unwrap();
}

PlayerMainAction GameTurn::PromptPlayerMainMenu() {
  MenuPrompt<PlayerMainAction> prompt;
  prompt.AddOptions(AllMainActions());
  if (!CanBuyAnimal()) prompt.RemoveOption(PlayerMainAction::BuyAnimal);
  return prompt().Unwrap();
}

bool GameTurn::CanBuyAnimal() const {
  return animals_bought_.MapCRef<bool>([&](AnimalPurchase p) {
      return p.second < MAX_ANIMAL_PURCHASES;
  }).UnwrapOr(true);
}
