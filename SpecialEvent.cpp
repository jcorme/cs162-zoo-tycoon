#include "SpecialEvent.h"

SpecialEvent::SpecialEvent(const Zoo &zoo, SpecialEventType t):
    zoo_(zoo), type_(t)
{
  SetValueBasedOnEvent();
}

SpecialEvent::SpecialEvent(const Zoo &zoo):
    zoo_(zoo), type_(RandomEventType())
{
  SetValueBasedOnEvent();
}

SpecialEvent::SpecialEvent(const Zoo &zoo, FoodType t):
    zoo_(zoo), type_(BiasedEventTypeFromFood(t))
{
  SetValueBasedOnEvent();
}

SpecialEvent::SpecialEvent(const SpecialEvent &s):
    zoo_(s.zoo_), type_(s.type_)
{
  switch (s.type_)
  {
    case SpecialEventType::SickAnimal:
      sick_animal_ = s.sick_animal_;
    case SpecialEventType::AnimalBirth:
      animal_birth_ = s.animal_birth_;
    case SpecialEventType::ZooAttendanceBoom:
      monkey_bonus_revenue_ = s.monkey_bonus_revenue_;
    default: break;
  }
}

SpecialEvent::~SpecialEvent()
{
  switch (type_)
  {
    case SpecialEventType::SickAnimal:
      sick_animal_.~Option();
      break;
    case SpecialEventType::AnimalBirth:
      animal_birth_.~Option();
      break;
    default: break;
  }
}

Option<std::reference_wrapper<const Animal>> SpecialEvent::sick_animal() const
{
  if (type_ != SpecialEventType::SickAnimal)
    return None;

  return sick_animal_;
}

Option<std::reference_wrapper<const Animal>> SpecialEvent::animal_birth() const
{
  if (type_ != SpecialEventType::AnimalBirth)
    return None;

  return animal_birth_;
};

Option<unsigned> SpecialEvent::monkey_bonus_revenue() const {
  if (type_ != SpecialEventType::ZooAttendanceBoom)
    return None;

  return monkey_bonus_revenue_;
}

Option<CAnimalRef> SpecialEvent::RandomSickAnimal()
{
  return ChooseRandomAnimal(zoo_.Animals());
}

Option<CAnimalRef> SpecialEvent::RandomAdultAnimal()
{
  std::vector<CAnimalRef> adult_animals =
      zoo_.AdultAnimals();
  auto fwd_it = std::remove_if(adult_animals.begin(), adult_animals.end(),
      [](CAnimalRef &a) {
          return !a.get().IsAdult();
      });
  adult_animals.erase(fwd_it, adult_animals.end());
  return ChooseRandomAnimal(adult_animals);
}

Option<CAnimalRef> SpecialEvent::ChooseRandomAnimal(
    std::vector<CAnimalRef> animals)
{
  using VecSizeT = std::vector<CAnimalRef>::size_type;
  if (animals.empty()) return None;
  std::uniform_int_distribution<VecSizeT> uni(0, animals.size() - 1);
  return animals[uni(rng_engine_)];
}

SpecialEventType SpecialEvent::RandomEventType()
{
  std::uniform_int_distribution<unsigned> uni(0, MAX_SPECIAL_EVENT_TYPE_INT);
  return static_cast<SpecialEventType>(uni(rng_engine_));
}

SpecialEventType SpecialEvent::BiasedEventTypeFromFood(FoodType t)
{
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
       i != (MAX_SPECIAL_EVENT_TYPE_INT + 1); ++i)
  {
    if (i == index_for_sickness) weights.push_back(sickness_weight);
    else weights.push_back(1.0);
  }

  std::discrete_distribution<unsigned> dis(weights.begin(), weights.end());
  return static_cast<SpecialEventType>(dis(rng_engine_));
}

void SpecialEvent::SetValueBasedOnEvent()
{
  switch (type_)
  {
    case SpecialEventType::SickAnimal:
      sick_animal_ = RandomSickAnimal();
      break;
    case SpecialEventType::AnimalBirth:
      animal_birth_ = RandomAdultAnimal();
      break;
    case SpecialEventType::ZooAttendanceBoom:
      monkey_bonus_revenue_ = RandomBonusRevenue();
      break;
    default: break;
  }
}

unsigned SpecialEvent::RandomBonusRevenue()
{
  std::uniform_int_distribution<unsigned> uni(
      MIN_EXTRA_BONUS_REVENUE, MAX_EXTRA_BONUS_REVENUE);
  return uni(rng_engine_);
}

std::ostream &operator<<(std::ostream &os, const SpecialEvent &s)
{
  os << "TO BE IMPLEMENTED." << std::endl;
  return os;
}
