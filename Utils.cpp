#include <random>
#include "Utils.h"

std::vector<uint32_t> GenerateSeedData(unsigned n_uints) {
  std::vector<uint32_t> seed_data(n_uints);
  std::random_device r;
  std::generate(seed_data.begin(), seed_data.end(), std::ref(r));
  return seed_data;
}

std::mt19937 MakeRngEngine() {
  std::vector<uint32_t> seed_data = GenerateSeedData();
  std::seed_seq seed(seed_data.begin(), seed_data.end());
  return std::mt19937(seed);
}
