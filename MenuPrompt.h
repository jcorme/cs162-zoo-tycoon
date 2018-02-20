#ifndef ZOO_TYCOON_MENUPROMPT_H
#define ZOO_TYCOON_MENUPROMPT_H
/*********************************************************************
** Program Filename: MenuPrompt.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the MenuPrompt template class and its related
 * members.
** Input: None
** Output: None
*********************************************************************/


#include <algorithm>
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <sstream>
#include "PlayerAction.h"
#include "Option.h"
#include "Utils.h"

// MenuPrompt is a (kind of) generic interface for getting input from the
// user. It takes a list of enum values, all of which must be of the same type,
// and asks the user to select one of them by entering in a number.
// The numbers are sequential, beginning from 1, increasing by one until
// there are no more options.
// There are two restrictions on T:
//    1. T must implement the comparison operators.
//    2. T must have an ActionString specialization, with a corresponding
//       Strings member map.
template <class T>
class MenuPrompt {
  // This doesn't actually work. There's no clean/sane way to check that
  // an explicit specialization of ActionString, with a Strings member,
  // exists in C++11. The static assertion doesn't technically fail, but the
  // compiler will still throw an error if no ActionString specialization for
  // T exists because ActionString<T>, and consequently
  // ActionString<T>::Strings, doesn't exist.
  static_assert(
      std::is_object<decltype(ActionString<T>::Strings)>::value,
      "T must have an explicit ActionString specialization with a Strings map"
  );

  public:
    using ValidationFn = std::function<bool(T)>;

    MenuPrompt() = default;
    // enable_cancel is whether there should be an extra option (0) that allows
    // the user to exit the current menu.
    explicit MenuPrompt(bool enable_cancel): enable_cancel_(enable_cancel) {}
    MenuPrompt(std::initializer_list<T> options);

    void AddOption(T option) { options_.push_back(option); }
    void AddOptions(std::vector<T> options);
    void OverrideStrings(ActionStringMap<T> overrides);
    void RemoveOption(T option);
    void SetValidationFn(ValidationFn fn) { custom_validation_fn_ = fn; }

    Option<T> operator()(
        Option<std::string> prompt_msg = None,
        Option<std::string> fail_msg = None);

  private:
    std::vector<T> options_;
    // Overrides the default printed option text.
    ActionStringMap<T> override_map_;
    bool enable_cancel_ = false;

    // Overrides the default function that validates user input.
    Option<ValidationFn> custom_validation_fn_;

    void SortOptions();
    void EraseDuplicateOptions();

    const std::string &DefaultStringFor(T option) const;
    std::string OptionsAsString() const;
    const std::string &StringFor(T option) const;
};

/*********************************************************************
** Function: MenuPrompt
** Description: Constructor for the MenuPrompt template class.
** Parameters: options is a list of options to give to the user.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
MenuPrompt<T>::MenuPrompt(std::initializer_list<T> options) {
  for (const auto &o : options)
    options_.push_back(o);
}

/*********************************************************************
** Function: AddOptions
** Description: Adds the options from the vector to print to the user.
** Parameters: options to add.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
void MenuPrompt<T>::AddOptions(std::vector<T> options) {
  for (const auto &o : options)
    AddOption(o);
}

/*********************************************************************
** Function: OverrideStrings
** Description: Overrides some, or all, of the default printed option text.
** Parameters: overrides is the map of overrides.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
void MenuPrompt<T>::OverrideStrings(ActionStringMap<T> overrides) {
  override_map_ = overrides;
}

/*********************************************************************
** Function: RemoveOption
** Description: Remove the provided option from the options printed to the user.
** Parameters: option is the option to remove.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
void MenuPrompt<T>::RemoveOption(T option) {
  options_.erase(
      std::remove(options_.begin(), options_.end(), option), options_.end());
}

/*********************************************************************
** Function: SortOptions
** Description: Sorts the provided options from least to greatest, however
 * that may be defined by the type.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
void MenuPrompt<T>::SortOptions()
{
  std::sort(options_.begin(), options_.end(), [](const T &t1, const T &t2) {
      return static_cast<unsigned>(t1) < static_cast<unsigned>(t2);
  });
}

/*********************************************************************
** Function: EraseDuplicateOptions
** Description: Removes duplicate options.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
void MenuPrompt<T>::EraseDuplicateOptions() {
  auto fwd_it = std::unique(options_.begin(), options_.end());
  options_.erase(fwd_it, options_.end());
}

/*********************************************************************
** Function: DefaultStringFor
** Description: Retrieves the default option text for the given option.
** Parameters: option is the option to retrieve the text for.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
const std::string &MenuPrompt<T>::DefaultStringFor(T option) const {
  return ActionString<T>::Strings.at(option);
}

/*********************************************************************
** Function: OptionsAsString
** Description: Converts the given options into a string presentable to
 * the user.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
std::string MenuPrompt<T>::OptionsAsString() const {
  std::ostringstream oss;

  if (enable_cancel_)
    oss << "0) Cancel" << '\n';

  for (decltype(options_.size()) i = 0; i != options_.size(); ++i)
    oss << i + 1 << ") " << StringFor(options_[i]) << '\n';

  return oss.str();
}

/*********************************************************************
** Function: StringFor
** Description: Provides the option text for the given option, checking
 * first if it has been overriden and returning the default text if not.
** Parameters: optoin is the option to retrieve the text for.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
const std::string &MenuPrompt<T>::StringFor(T option) const {
  if (override_map_.empty()) return DefaultStringFor(option);

  if (override_map_.find(option) != override_map_.cend()) {
    return override_map_.at(option);
  }

  return DefaultStringFor(option);
}

/*********************************************************************
** Function: operator()
** Description: Overloads the function call operator; this prompts the
 * user with the given options and returns their choice.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
Option<T> MenuPrompt<T>::operator()(
    Option<std::string> prompt_msg, Option<std::string> fail_msg) {
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


#endif //ZOO_TYCOON_MENUPROMPT_H
