#include "PlayerAction.h"

const ActionStringMap<PlayerMainAction>
    ActionString<PlayerMainAction>::Strings = {
        { PlayerMainAction::BuyAnimal, "Buy an animal." },
        { PlayerMainAction::ViewZooAnimals, "List the animals in your zoo." },
        { PlayerMainAction::CheckBank, "Check your bank account." },
        { PlayerMainAction::PrintGameState,
            "Print the current state of the game." },
        { PlayerMainAction::EndTurn, "End the current turn." },
        { PlayerMainAction::QuitGame, "Quit game." }
};

const ActionStringMap<AnimalSpecies>
    ActionString<AnimalSpecies>::Strings = {
        { AnimalSpecies::Monkey, "Purchase a monkey." },
        { AnimalSpecies::SeaOtter, "Purchase a sea otter." },
        { AnimalSpecies::Sloth, "Purchase a sloth."},
        { AnimalSpecies::Elephant, "Purchase an elephant."}
};

const ActionStringMap<FoodType> ActionString<FoodType>::Strings = {
    { FoodType::Regular, "Feed your animals normal food." },
    { FoodType::Premium, "Feed your animals premium food; "
        "twice the cost, but reduces chance of sickness by two." },
    { FoodType::Cheap, "Feed your animals cheap food; "
        "half the cost, but increases chance of sickness by two." }
};

const ActionStringMap<unsigned> ActionString<unsigned>::Strings = {};

std::vector<PlayerMainAction> AllMainActions()
{
  return ActionStringMapKeys(ActionString<PlayerMainAction>::Strings);
}

std::vector<FoodType> AllFoodOptions()
{
  return ActionStringMapKeys(ActionString<FoodType>::Strings);
}
