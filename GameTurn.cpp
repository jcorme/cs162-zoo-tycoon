#include <iostream>
#include "GameTurn.h"
#include "MenuPrompt.h"

unsigned GameTurn::day_ = 0;
FoodType GameTurn::food_type_ = FoodType::Regular;

GameTurn::GameTurn(Player &player, double base_food_cost):
    player_(player), zoo_(player.zoo()),
    special_event_(SpecialEvent(player.zoo(), food_type_)),
    base_food_cost_(base_food_cost), monkey_bonus_revenue_(None)
{
  food_type_= PromptPlayerFoodType();
  std::cout << "\n\n" << std::endl;
  ++day_;
}

GameTurnResult GameTurn::Run()
{
  zoo_.IncrementAnimalAges();
  PrintGameState();
  FeedAnimals();

  Option<GameTurnResult> handle_result = HandleSpecialEvent();
  if (handle_result.IsSome())
  {
    GameTurnResult result = handle_result.Unwrap();
    if (result == GameTurnResult::PlayerBankrupt)
      return result;
  }

  for (;;)
  {
    PlayerMainAction action = PromptPlayerMainMenu();
    if (action == PlayerMainAction::EndTurn) break;
    else if (action == PlayerMainAction::QuitGame) return GameTurnResult::Quit;
    // Currently, not worried about the return value.
    else HandleMainAction(action);
  }

  GivePlayerRevenue();

  return GameTurnResult::Continue;
}

Option<GameTurnResult> GameTurn::HandleSpecialEvent()
{
  switch (special_event_.type())
  {
    case SpecialEventType::AnimalBirth: {
      Option<CAnimalRef> possible_parent = special_event_.animal_birth();
      return AnimalBirth(possible_parent);
    }
    case SpecialEventType::SickAnimal: {
      Option<CAnimalRef> possible_sick_animal = special_event_.sick_animal();
      return SickAnimal(possible_sick_animal);
    }
    case SpecialEventType::ZooAttendanceBoom:
      monkey_bonus_revenue_ = special_event_.monkey_bonus_revenue();
      std::cout << "There is a zoo attendance boom today! Each monkey will "
                << "generate an extra $" << monkey_bonus_revenue_.CUnwrapRef()
                << " in revenue today!\n";
      return None;
    default: return None;
  }
}

Option<GameTurnResult> GameTurn::HandleMainAction(PlayerMainAction action)
{
  switch (action) {
    case PlayerMainAction::BuyAnimal:
    {
      Option<std::pair<AnimalSpecies, unsigned>> p_purchase =
          PromptPlayerBuyAnimal();
      if (p_purchase.IsSome())
      {
        auto purchase = p_purchase.Unwrap();
        if (!PlayerBuyAnimal(purchase.first, purchase.second))
        {
          std::cout << "Unable to make purchase; "
                    << "probably due to insufficient funds (let's hope).\n";
          return GameTurnResult::InsufficientFunds;
        }
      }
      break;
    }
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

Option<GameTurnResult> GameTurn::AnimalBirth(
    Option<CAnimalRef> possible_parent)
{
  if (possible_parent.IsNone()) return GameTurnResult::MissingAnimals;
  CAnimalRef parent = possible_parent.Unwrap();
  std::cout << "An adult " << parent.get().name() << " gave birth to "
            << parent.get().babies_per_birth() << " babies!\n";
  std::vector<CAnimalRef> babies = zoo_.AnimalGiveBirth(parent);
  for (const auto &b : babies)
  {
    if (!player_.FeedAnimal(b, food_type_, base_food_cost_))
    {
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

Option<GameTurnResult> GameTurn::SickAnimal(
    Option<CAnimalRef> possible_sick_animal)
{
  if (possible_sick_animal.IsNone()) return GameTurnResult::MissingAnimals;
  CAnimalRef sick_animal = possible_sick_animal.Unwrap();
  std::cout << "A " << sick_animal.get().name() << " fell sick!\n";
  if (!player_.CareForSickAnimal(sick_animal))
  {
    std::cout << "Since you cannot afford to pay for their medical costs, "
              << "the " << sick_animal.get().name() << " has died.\n";
    zoo_.RemoveAnimal(sick_animal);
  }
  else
    std::cout << "You paid $" << sick_animal.get().SickCareCost()
              << " in medical costs to treat the " << sick_animal.get().name()
              << ".\n";

  return None;
}

PlayerMainAction GameTurn::PromptPlayerMainMenu()
{
  MenuPrompt<PlayerMainAction> prompt;
  prompt.AddOptions(AllMainActions());
  if (!CanBuyAnimal()) prompt.RemoveOption(PlayerMainAction::BuyAnimal);
  return prompt().Unwrap();
}

Option<std::pair<AnimalSpecies, unsigned>> GameTurn::PromptPlayerBuyAnimal()
{
  if (!CanBuyAnimal()) return None;

  std::ostringstream oss;
  std::vector<AnimalSpecies> animal_options;
  if (animals_bought_.IsSome())
  {
    const std::pair<AnimalSpecies, unsigned> &animals_bought =
        animals_bought_.CUnwrapRef();
    std::string animal_type =  AnimalSpeciesToString(animals_bought.first);
    unsigned number_bought = animals_bought.second;
    oss << "\nYou've already purchased " << number_bought << ' ' << animal_type
        << " this turn, so the only thing you can buy is "
        << MAX_ANIMAL_PURCHASES - number_bought << " more " << animal_type
        << ".\n";

    oss << "What would you like to do?";

    animal_options.push_back(animals_bought.first);
  }

  if (animal_options.empty())
    animal_options = AllSpecies();

  MenuPrompt<AnimalSpecies> prompt(true);
  prompt.AddOptions(animal_options);
  Option<AnimalSpecies> p_species = prompt(oss.str());
  if (p_species.IsNone()) return None;

  AnimalSpecies species = p_species.Unwrap();
  Option<unsigned> p_qty = PromptPlayerPurchaseQuantity(species);
  if (p_qty.IsNone()) return None;
  return std::make_pair(species, p_qty.Unwrap());
}

Option<unsigned> GameTurn::PromptPlayerPurchaseQuantity(AnimalSpecies s) const
{
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
  return prompt();
}

bool GameTurn::PlayerBuyAnimal(AnimalSpecies s, unsigned qty)
{
  if (!player_.BuyAnimals(s, qty)) return false;

  std::cout << "\nYou purchased " << qty << ' ' << AnimalSpeciesToString(s)
            << "s!\n";

  if (animals_bought_.IsSome())
    animals_bought_.UnwrapRef().second += qty;
  else
    animals_bought_ = std::make_pair(s, qty);

  return true;
}

void GameTurn::GivePlayerRevenue()
{
  std::string n_animals = std::to_string(zoo_.NumberOfAnimals());
  std::string desc = "Daily zoo revenue from " + n_animals + " animals";
  double total_revenue = zoo_.TotalDailyRevenue(monkey_bonus_revenue_);
  player_.AddMoney(total_revenue, desc);
  std::cout << "\nThe zoo made $" << total_revenue << " today, bringing your "
            << "bank balance to $" << player_.MoneyRemaining() << ".\n";
}

void GameTurn::PrintGameState() const
{
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

Option<GameTurnResult> GameTurn::FeedAnimals()
{
  if (!player_.FeedAnimals(food_type_, base_food_cost_))
    return GameTurnResult::PlayerBankrupt;
  double feeding_cost = zoo_.FeedingCost(food_type_, base_food_cost_);
  if (feeding_cost > 0)
    std::cout << "Successfully fed all the animals; paid $" << feeding_cost
              << '.' << std::endl;
  return None;
}

bool GameTurn::CanBuyAnimal() const
{
  if (animals_bought_.IsNone()) return true;
  const std::pair<AnimalSpecies, unsigned> &animals_bought =
      animals_bought_.CUnwrapRef();
  return animals_bought.second < MAX_ANIMAL_PURCHASES;
}

FoodType GameTurn::PromptPlayerFoodType() const
{
  MenuPrompt<FoodType> prompt;
  prompt.AddOptions(AllFoodOptions());
  std::cout << "\nWhat food would you like to feed your animals today?\n";
  return prompt().Unwrap();
}
