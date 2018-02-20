#ifndef ZOO_TYCOON_SPECIALEVENT_H
#define ZOO_TYCOON_SPECIALEVENT_H
/*********************************************************************
** Program Filename: SpecialEvent.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the SpecialEvent class and its related members. 
** Input: None
** Output: None
*********************************************************************/


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

class SpecialEvent {
  public:
    SpecialEvent(const Zoo &zoo, SpecialEventType t);
    explicit SpecialEvent(const Zoo &zoo);
    SpecialEvent(const Zoo &zoo, FoodType t);
    SpecialEvent(const SpecialEvent &s);

    ~SpecialEvent();

    // These accessors return Option because their values only exist if the
    // event type is their corresponding type.
    Option<CAnimalRef> animal_birth() const;
    Option<unsigned> monkey_bonus_revenue() const;
    Option<CAnimalRef> sick_animal() const;
    SpecialEventType type() const { return type_; }


  private:
    std::mt19937 rng_engine_ = MakeRngEngine();

    const Zoo &zoo_;

    SpecialEventType type_;

    union {
      void *stub_ = nullptr;
      Option<CAnimalRef> animal_birth_;
      unsigned monkey_bonus_revenue_;
      Option<CAnimalRef> sick_animal_;
    };

    SpecialEventType BiasedEventTypeFromFood(FoodType t);
    Option<CAnimalRef> ChooseRandomAnimal(
        std::vector<CAnimalRef> animals);
    Option<CAnimalRef> RandomAdultAnimal();
    unsigned RandomBonusRevenue();
    SpecialEventType RandomEventType();
    Option<CAnimalRef> RandomSickAnimal();

    void SetValueBasedOnEvent();
};


#endif //ZOO_TYCOON_SPECIALEVENT_H
