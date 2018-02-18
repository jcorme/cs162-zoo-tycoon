#ifndef ZOO_TYCOON_UTILS_H
#define ZOO_TYCOON_UTILS_H


#include <algorithm>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Option.h"

template <class T>
using InputValidationFn = std::function<bool(const T &)>;

// Default amount of seed data to generate for RNG.
const int MT19937_MAX_UINTS = 624;

std::vector<uint32_t> GenerateSeedData(unsigned n_uints = MT19937_MAX_UINTS);
std::mt19937 MakeRngEngine();

// Avoids need for hash specialization,
// per https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
struct EnumClassHash
{
  template <class T>
  std::size_t operator()(T t) const
  {
    return static_cast<std::size_t>(t);
  }
};

template <class T>
using ActionStringMap = std::unordered_map<T, std::string, EnumClassHash>;

template <class T>
std::vector<T> ActionStringMapKeys(const ActionStringMap<T> &map)
{
  std::vector<T> keys;
  keys.reserve(map.size());
  for (const auto &m : map)
    keys.push_back(m.first);
  return keys;
}

template <class T>
bool StreamGetT(std::istream &is, T &t)
{
  std::string line;
  std::getline(is, line);
  std::istringstream iss(line);
  iss >> t;
  char c;
  if (iss.fail() || iss.get(c))
    return false;
  return true;
}

template <class T>
T PromptUntilValid(
    const std::string &prompt_msg,
    InputValidationFn<T> is_valid,
    const Option<std::string> &fail_msg = None)
{
  T t;

  for (;;)
  {
    std::cout << prompt_msg;
    if (StreamGetT(std::cin, t))
    {
      if (is_valid(t))
        break;
    }
    else
      if (fail_msg.IsSome())
        std::cout << fail_msg.CUnwrapRef() << std::endl;
  }

  return t;
}

template <class T>
T PromptUntilValid(
    const std::string &prompt_msg,
    InputValidationFn<T> is_valid,
    const std::string &p_fail_msg)
{
  return PromptUntilValid<T>(prompt_msg, is_valid, Option<std::string>(p_fail_msg));
}

template <class T>
T PromptUntilValid(
    const std::string &prompt_msg,
    std::initializer_list<T> valid_inputs)
{
  return PromptUntilValid<T>(
      prompt_msg,
      [&](const T &t) {
        for (const auto &i : valid_inputs)
          if (i == t) return true;
        return false;
      }
  );
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


#endif //ZOO_TYCOON_UTILS_H
