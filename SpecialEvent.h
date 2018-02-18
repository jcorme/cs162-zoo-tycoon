#ifndef ZOO_TYCOON_SPECIALEVENT_H
#define ZOO_TYCOON_SPECIALEVENT_H


#include "Utils.h"
#include "Zoo.h"
#include "FoodType.h"

static constexpr unsigned MAX_SPECIAL_EVENT_TYPE_INT = 3;
static constexpr unsigned MIN_EXTRA_BONUS_REVENUE = 250;
static constexpr unsigned MAX_EXTRA_BONUS_REVENUE = 500;

enum class SpecialEventType
{
    SickAnimal,
    AnimalBirth,
    ZooAttendanceBoom,
    NoSpecialEvent
};

class SpecialEvent
{
  friend std::ostream &operator<<(std::ostream &os, const SpecialEvent &s);

  public:
    SpecialEvent(const Zoo &zoo, SpecialEventType t);
    explicit SpecialEvent(const Zoo &zoo);
    SpecialEvent(const Zoo &zoo, FoodType t);
    explicit SpecialEvent(const SpecialEvent &s);

    ~SpecialEvent();

    SpecialEventType type() const { return type_; }

    // These accessors return Option because their values only exist if the
    // event type is their corresponding type.
    Option<CAnimalRef> sick_animal() const;
    Option<CAnimalRef> animal_birth() const;
    Option<unsigned> monkey_bonus_revenue() const;

  private:
    std::mt19937 rng_engine_ = MakeRngEngine();

    const Zoo &zoo_;

    SpecialEventType type_;

    union
    {
      void *stub_ = nullptr;
      Option<CAnimalRef> sick_animal_;
      Option<CAnimalRef> animal_birth_;
      unsigned monkey_bonus_revenue_;
    };

    Option<CAnimalRef> RandomSickAnimal();
    Option<CAnimalRef> RandomAdultAnimal();

    Option<CAnimalRef> ChooseRandomAnimal(
        std::vector<CAnimalRef> animals);

    SpecialEventType RandomEventType();
    SpecialEventType BiasedEventTypeFromFood(FoodType t);
    void SetValueBasedOnEvent();
    unsigned RandomBonusRevenue();
};

std::ostream &operator<<(std::ostream &os, const SpecialEvent &s);


#endif //ZOO_TYCOON_SPECIALEVENT_H
