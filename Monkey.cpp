#include "Monkey.h"

Monkey::Monkey(unsigned age):
    Animal("Monkey", age, MONKEY_UNIT_COST,
           MONKEY_BABIES_PER_BIRTH,
           MONKEY_FOOD_COST_MULTIPLIER,
           MONKEY_REVENUE_PCT) {}

double Monkey::DailyRevenue(Option<unsigned> bonus_revenue) const {
  double revenue = Animal::DailyRevenue(None);
  if (bonus_revenue.IsSome())
    revenue += bonus_revenue.CUnwrapRef();
  return revenue;
}

AnimalsVec Monkey::GiveBirth() const {
  return AnimalGiveBirth<Monkey>(this);
}
