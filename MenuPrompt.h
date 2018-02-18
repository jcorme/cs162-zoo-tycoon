#ifndef ZOO_TYCOON_MENUPROMPT_H
#define ZOO_TYCOON_MENUPROMPT_H


#include <algorithm>
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <sstream>
#include "PlayerAction.h"
#include "Option.h"
#include "Utils.h"

template <class T>
class MenuPrompt
{
//  static_assert(
      // Enum classes can't be implicitly converted to an int, while
      // regular enums can.
//      (std::is_enum<T>::value && !std::is_convertible<T, int>::value),
//      "An enum class is required to use the MenuPrompt.");

  public:
    using ValidationFn = std::function<bool(T)>;

    MenuPrompt() = default;
    explicit MenuPrompt(bool enable_cancel): enable_cancel_(enable_cancel) {}
    MenuPrompt(std::initializer_list<T> options);

    void AddOption(T option) { options_.push_back(option); }
    void AddOptions(std::vector<T> options);
    void RemoveOption(T option);
    void OverrideStrings(ActionStringMap<T> overrides);
    void SetValidationFn(ValidationFn fn) { custom_validation_fn_ = fn; }

    Option<T> operator()(
        Option<std::string> prompt_msg = None, Option<std::string> fail_msg = None);

  private:
    std::vector<T> options_;
    ActionStringMap<T> override_map_;
    bool enable_cancel_ = false;

    Option<ValidationFn> custom_validation_fn_;

    void SortOptions();
    void EraseDuplicateOptions();

    std::string OptionsAsString() const;
    const std::string &StringFor(T option) const;
    const std::string &DefaultStringFor(T option) const;
};

template <class T>
MenuPrompt<T>::MenuPrompt(std::initializer_list<T> options)
{
  for (const auto &o : options)
    options_.push_back(o);
}

template <class T>
void MenuPrompt<T>::AddOptions(std::vector<T> options)
{
  for (const auto &o : options)
    AddOption(o);
}

template <class T>
void MenuPrompt<T>::RemoveOption(T option)
{
  options_.erase(
      std::remove(options_.begin(), options_.end(), option), options_.end());
}

template <class T>
void MenuPrompt<T>::OverrideStrings(ActionStringMap<T> overrides)
{
  override_map_ = overrides;
}

template <class T>
Option<T> MenuPrompt<T>::operator()(
    Option<std::string> prompt_msg, Option<std::string> fail_msg)
{
  SortOptions();
  EraseDuplicateOptions();

  if (options_.empty()) return None;

  if (prompt_msg.IsSome())
    std::cout << prompt_msg.CUnwrapRef() << "\n\n";
  else
    std::cout << "\nChoose an option from below:\n\n";

  std::string options_str = OptionsAsString();
  std::cout << options_str << std::endl;
  auto max_opt = options_.size();
  unsigned choice = PromptUntilValid<unsigned>(
      "Enter option: ",
      [&](const unsigned &opt) {
          if (enable_cancel_ && opt == 0) return true;
          bool in_range = opt > 0 && opt <= max_opt;
          if (custom_validation_fn_.IsSome())
            return in_range &&
                custom_validation_fn_.CUnwrapRef()(options_[opt - 1]);
          return in_range;
      }
  );

  if (!choice) return None;

  return options_[choice - 1];
}

template <class T>
void MenuPrompt<T>::SortOptions()
{
  std::sort(options_.begin(), options_.end(), [](const T &t1, const T &t2) {
      return static_cast<unsigned>(t1) < static_cast<unsigned>(t2);
  });
}

template <class T>
void MenuPrompt<T>::EraseDuplicateOptions()
{
  auto fwd_it = std::unique(options_.begin(), options_.end());
  options_.erase(fwd_it, options_.end());
}

template <class T>
std::string MenuPrompt<T>::OptionsAsString() const
{
  std::ostringstream oss;
  if (enable_cancel_)
    oss << "0) Cancel" << '\n';
  for (decltype(options_.size()) i = 0; i != options_.size(); ++i)
    oss << i + 1 << ") " << StringFor(options_[i]) << '\n';
  return oss.str();
}

template <class T>
const std::string &MenuPrompt<T>::StringFor(T option) const
{
  if (override_map_.empty()) return DefaultStringFor(option);
  if (override_map_.find(option) != override_map_.cend()) {
    return override_map_.at(option);
  }
  return DefaultStringFor(option);
}

template <class T>
const std::string &MenuPrompt<T>::DefaultStringFor(T option) const
{
  return ActionString<T>::Strings.at(option);
}


#endif //ZOO_TYCOON_MENUPROMPT_H
