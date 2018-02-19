#include <iterator>
#include <random>
#include "Utils.h"

std::mt19937 MakeRngEngine() {
  std::mt19937::result_type seed_data[std::mt19937::state_size];
  std::random_device r;
  std::generate(std::begin(seed_data), std::end(seed_data), std::ref(r));
  std::seed_seq seed(std::begin(seed_data), std::end(seed_data));
  return std::mt19937(seed);
}
