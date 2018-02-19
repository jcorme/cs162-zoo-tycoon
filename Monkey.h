#ifndef ZOO_TYCOON_MONKEY_H
#define ZOO_TYCOON_MONKEY_H


#include "Animal.h"

static constexpr unsigned MONKEY_UNIT_COST = 15000;
static constexpr unsigned MONKEY_BABIES_PER_BIRTH = 1;
static constexpr unsigned MONKEY_FOOD_COST_MULTIPLIER = 4;
static constexpr double MONKEY_REVENUE_PCT = 0.10;

class Monkey: public Animal {
  public:
    explicit Monkey(unsigned age);
    Monkey(): Monkey(0) {}

    virtual double DailyRevenue(Option<unsigned> bonus_revenue) const override;

    virtual std::vector<std::unique_ptr<Animal>> GiveBirth() const override;
};


#endif //ZOO_TYCOON_MONKEY_H
