/*********************************************************************
** Program Filename: SpecialEvent.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the SpecialEvent class
 * and in the SpecialEvent header.
** Input: None
** Output: None
*********************************************************************/
#include "SpecialEvent.h"

/*********************************************************************
** Function: SpecialEvent
** Description: Constructor for SpecialEvent class.
** Parameters: zoo is the game's main Zoo object; t is the event type
 * to use for this instantiation.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEvent::SpecialEvent(const Zoo &zoo, SpecialEventType t):
    zoo_(zoo), type_(t) {
  SetValueBasedOnEvent();
}

/*********************************************************************
** Function: SpecialEvent
** Description: Constructor for the SpecialEvent class.
** Parameters: zoo is the game's main Zoo object.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEvent::SpecialEvent(const Zoo &zoo):
    zoo_(zoo), type_(RandomEventType()) {
  SetValueBasedOnEvent();
}

/*********************************************************************
** Function: SpecialEvent
** Description: Constructor for the SpecialEvent class.
** Parameters: zoo is the game's main Zoo object; t is the food type
 * being fed to animals, which is used to bias the probability of certain
 * events.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEvent::SpecialEvent(const Zoo &zoo, FoodType t):
    zoo_(zoo), type_(BiasedEventTypeFromFood(t)) {
  SetValueBasedOnEvent();
}

/*********************************************************************
** Function: SpecialEvent
** Description: Copy constructor for the SpecialEvent class.
** Parameters: s is the SpecialEvent object to copy.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEvent::SpecialEvent(const SpecialEvent &s):
    zoo_(s.zoo_), type_(s.type_) {
  switch (s.type_) {
    case SpecialEventType::SickAnimal:
      new (&sick_animal_) Option<CAnimalRef>(s.sick_animal_);
    case SpecialEventType::AnimalBirth:
      new (&animal_birth_) Option<CAnimalRef>(s.animal_birth_);
    case SpecialEventType::ZooAttendanceBoom:
      monkey_bonus_revenue_ = s.monkey_bonus_revenue_;
    default: break;
  }
}

/*********************************************************************
** Function: ~SpecialEvent
** Description: Destructor for the SpecialEvent class.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEvent::~SpecialEvent() {
  switch (type_) {
    case SpecialEventType::SickAnimal:
      sick_animal_.~Option();
      break;
    case SpecialEventType::AnimalBirth:
      animal_birth_.~Option();
      break;
    default: break;
  }
}

/*********************************************************************
** Function: animal_birth
** Description: Returns the animal that has been chosen to give birth; if
 * that is the chosen event and an adult animal exists.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Option<CAnimalRef> SpecialEvent::animal_birth() const {
  if (type_ != SpecialEventType::AnimalBirth)
    return None;

  return animal_birth_;
};

/*********************************************************************
** Function: monkey_bonus_revenue
** Description: Returns the amount of bonus revenue monkeys should generate
 * due to an attendance boom; only exists if ZooAttendanceBoom is the chosen
 * event.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Option<unsigned> SpecialEvent::monkey_bonus_revenue() const {
  if (type_ != SpecialEventType::ZooAttendanceBoom)
    return None;

  return monkey_bonus_revenue_;
}

/*********************************************************************
** Function: sick_animal
** Description: Returns the animal that has been chosen to become sick;
 * only exists if SickAnimal is the chosen event and there are animals in
 * the zoo.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Option<CAnimalRef> SpecialEvent::sick_animal() const {
  if (type_ != SpecialEventType::SickAnimal)
    return None;

  return sick_animal_;
}

/*********************************************************************
** Function: BiasedEventTypeFromFood
** Description: Selects a random event type based on the type of food
 * being fed to the animals. When the animals are fed cheap food, the
 * probability of the SickAnimal event doubles; when they are fed premium
 * food, that probability is halved.
** Parameters: t is the type of food being fed to the zoo animals.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEventType SpecialEvent::BiasedEventTypeFromFood(FoodType t) {
  double sickness_weight;
  unsigned m = MAX_SPECIAL_EVENT_TYPE_INT;

  if (t == FoodType::Premium)
    // Solve (1/2)(1/(m+1)) = (w/(m+w)) for w, where m is one less than the
    // number of possible events and w is the weight for the value
    // corresponding to the SickAnimal event.
    sickness_weight = static_cast<double>(m) / (2 * m + 1);
  else if (t == FoodType::Cheap)
    // Solve 2*(1/(m+1)) = (w/(m+w)) for w, where m and w are as above.
    sickness_weight = static_cast<double>(2 * m) / (m - 1);
  else
    return RandomEventType();

  unsigned index_for_sickness =
      static_cast<unsigned>(SpecialEventType::SickAnimal);
  std::vector<double> weights;
  weights.reserve(MAX_SPECIAL_EVENT_TYPE_INT + 1);
  for (decltype(weights.size()) i = 0;
       i != (MAX_SPECIAL_EVENT_TYPE_INT + 1); ++i) {
    if (i == index_for_sickness) weights.push_back(sickness_weight);
    else weights.push_back(1.0);
  }

  std::discrete_distribution<unsigned> dis(weights.begin(), weights.end());
  return static_cast<SpecialEventType>(dis(rng_engine_));
}

/*********************************************************************
** Function: ChooseRandomAnimal
** Description: Selects a random animal from the given vector.
** Parameters: animals is the vector to choose from.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Option<CAnimalRef> SpecialEvent::ChooseRandomAnimal(
    std::vector<CAnimalRef> animals) {
  using VecSizeT = std::vector<CAnimalRef>::size_type;
  if (animals.empty()) return None;
  std::uniform_int_distribution<VecSizeT> uni(0, animals.size() - 1);
  return animals[uni(rng_engine_)];
}

/*********************************************************************
** Function: RandomAdultAnimal
** Description: Chooses a random adult animal from the zoo, provided one
 * exists.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Option<CAnimalRef> SpecialEvent::RandomAdultAnimal() {
  std::vector<CAnimalRef> adult_animals = zoo_.AdultAnimals();
  return ChooseRandomAnimal(adult_animals);
}

/*********************************************************************
** Function: RandomBonusRevenue
** Description: Generates a random amount of bonus revenue for the attendance
 * boom event.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
unsigned SpecialEvent::RandomBonusRevenue() {
  std::uniform_int_distribution<unsigned> uni(
      MIN_EXTRA_BONUS_REVENUE, MAX_EXTRA_BONUS_REVENUE);
  return uni(rng_engine_);
}

/*********************************************************************
** Function: RandomEventType
** Description: Selects a random event type.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
SpecialEventType SpecialEvent::RandomEventType() {
  std::uniform_int_distribution<unsigned> uni(0, MAX_SPECIAL_EVENT_TYPE_INT);
  return static_cast<SpecialEventType>(uni(rng_engine_));
}

/*********************************************************************
** Function: RandomSickAnimal
** Description: Chooses a random zoo animal to get sick.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
Option<CAnimalRef> SpecialEvent::RandomSickAnimal() {
  return ChooseRandomAnimal(zoo_.Animals());
}

/*********************************************************************
** Function: SetValueBasedOnEvent
** Description: Used the constructor to set the corresponding value for an
 * event.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
void SpecialEvent::SetValueBasedOnEvent() {
  switch (type_) {
    case SpecialEventType::SickAnimal:
      new (&sick_animal_) Option<CAnimalRef>(RandomSickAnimal());
      break;
    case SpecialEventType::AnimalBirth:
      new (&animal_birth_) Option<CAnimalRef>(RandomAdultAnimal());
      break;
    case SpecialEventType::ZooAttendanceBoom:
      monkey_bonus_revenue_ = RandomBonusRevenue();
      break;
    default: break;
  }
}
