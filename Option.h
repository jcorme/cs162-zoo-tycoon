#ifndef ZOO_TYCOON_OPTION_H
#define ZOO_TYCOON_OPTION_H
/*********************************************************************
** Program Filename: Option.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the Option class and its related members.
** Input: None
** Output: None
*********************************************************************/


#include <functional>
#include <iostream>
#include <utility>
#include <exception>

// This is the exception thrown when an Option that is None or whose value has
// been moved is unwrapped.
class BadOptionAccess: public std::exception {
  public:
    explicit BadOptionAccess(const char *msg): msg_(msg) {}
    const char *what() const noexcept override { return msg_; }

  private:
    const char *msg_;
};

// The None value of None type allows easy construction of empty Option
// objects.
struct None {};
constexpr None None{};

// Option<T> represents an optional value (a value that may or may not exist).
// If the state of an option is Some, then it holds a value; if the state is
// None, then it does not hold a value. This type is used as the return type
// for functions that may fail to return a value.
template <class T>
class Option {
  public:
    Option(): has_value_(false), moved_(false), stub_(nullptr) {}
    Option(const T &t): has_value_(true), moved_(false), value_(t) {}
    Option(T &&t): has_value_(true), moved_(false), value_(std::move(t)) {}
    Option(const Option<T> &t);
    Option(Option<T> &&t) noexcept;
    Option(const struct None &):
        has_value_(false), moved_(false), stub_(nullptr) {}

    ~Option();

    explicit operator bool() const { return IsSome(); }

    bool IsNone() const { return !has_value_ || moved_ ; };
    bool IsSome() const { return has_value_ && !moved_; };

    inline const T &CUnwrapRef() const;
    inline const T &CUnwrapRefOr(const T &t) const;
    inline T Unwrap();
    inline T UnwrapOr(T t);
    inline T &UnwrapRef() const;

    template <class E>
    Option<E> AndThen(std::function<Option<E>(T)> f);

    template <class E>
    Option<E> Map(std::function<E(T)> f);

    template <class E>
    Option<E> MapCRef(std::function<E(const T &)> f) const;

    Option<T> &operator=(const Option<T> &rhs);

  private:
    bool has_value_;
    bool moved_;

    union {
      void* stub_;
      T value_;
    };

    inline void ThrowIfBad() const;
};

/*********************************************************************
** Function: Option
** Description: Copy constructor for the Option class template.
** Parameters: t is the Option<T> being copied.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
Option<T>::Option(const Option<T> &t):
    has_value_(false), moved_(false), stub_(nullptr) {
  has_value_ = t.has_value_;
  moved_ = t.moved_;

  if (t.IsSome())
    new (&value_) T(t.value_);
  else
    stub_ = nullptr;
}

/*********************************************************************
** Function: Option
** Description: Move constructor for the Option class template.
** Parameters: t is an rvalue reference (to the Option<T> being moved).
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
Option<T>::Option(Option<T> &&t) noexcept:
    has_value_(std::move(t.has_value_)), moved_(std::move(t.moved_)),
    stub_(nullptr) {
  if (t.IsSome())
    new (&value_) T(std::move(t.value_));
}

/*********************************************************************
** Function: ~Option
** Description: Destructor for the Option class template.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
Option<T>::~Option() {
  if (IsSome())
    value_.~T();
}

/*********************************************************************
** Function: CUnwrapRef
** Description: Returns a const reference to the value inside the optional,
 * PROVIDED IT EXISTS. This function will throw an exception if the value
 * has been moved or doesn't exist.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
const T &Option<T>::CUnwrapRef() const {
  ThrowIfBad();
  return value_;
}

/*********************************************************************
** Function: CUnwrapRefOr
** Description: Returns a reference to the value inside the optional if
 * it exists; otherwise, returns the argument.
** Parameters: t is the default value to return if the optional is None.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
const T &Option<T>::CUnwrapRefOr(const T &t) const {
  if (IsNone()) return t;
  return CUnwrapRef();
}

/*********************************************************************
** Function: Unwrap
** Description: Moves (returns) the value held by the optional if it exists;
 * this function will throw an exception if the value has already been
 * moved or no value exists.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
T Option<T>::Unwrap() {
  ThrowIfBad();
  moved_ = true;
  return std::move(value_);
}

/*********************************************************************
** Function: UnwrapOr
** Description: Moves (returns) the value held by the optional if it exists;
 * otherwise, returns the argument.
** Parameters: t is the default value to return if the optional is None.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
T Option<T>::UnwrapOr(T t)
{
  if (IsNone()) return t;
  return Unwrap();
}

/*********************************************************************
** Function: UnwrapRef
** Description: Returns a reference to the value inside the optional,
 * PROVIDED IT EXISTS. This function will throw an exception if the value
 * has been moved or doesn't exist.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
T &Option<T>::UnwrapRef() const {
  ThrowIfBad();
  // I'm not entirely sure why g++ 6.3.0 qualifies value_ with const.
  // This probably isn't the safest thing in the world...
  return const_cast<T &>(value_);
}

/*********************************************************************
** Function: AndThen
** Description: Transforms an Option<T> to an Option<E> by using the given
 * function (T -> Option<E>) to transform a T into Option<E>; returns None
 * if no value exists; moves the value if it does exist.
** Parameters: f is a function (T -> Option<E>).
** Pre-Conditions:
** Post-Conditions:
*********************************************************************/
template <class T>
template <class E>
Option<E> Option<T>::AndThen(std::function<Option<E>(T)> f) {
  if (IsNone()) return None;
  moved_ = true;
  return f(std::move(value_));
}

/*********************************************************************
** Function: Map
** Description: Transforms an Option<T> to an Option<E> by using the given
 * function (T -> E) to map T to E; returns None if no value exists; moves
 * the value if it does exist.
** Parameters: f is a function (T -> E).
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
template <class E>
Option<E> Option<T>::Map(std::function<E(T)> f) {
  if (IsNone()) return None;
  moved_ = true;
  return f(std::move(value_));
};

/*********************************************************************
** Function: MapCRef
** Description: Transforms an Option<T> to an Option<E> by using the given
 * function (T -> E) to map T to E; returns None if no value exists; does not
 * move the value in any case.
** Parameters: f is a function (T -> E).
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
template <class E>
Option<E> Option<T>::MapCRef(std::function<E(const T &)> f) const {
  if (IsNone()) return None;
  return f(value_);
};

/*********************************************************************
** Function: ThrowIfBad
** Description: Throws an exception if called and the value has been moved
 * or no value exists.
** Parameters: None
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
void Option<T>::ThrowIfBad() const {
  if (moved_)
    throw BadOptionAccess("Cannot unwrap moved value.");

  if (!has_value_)
    throw BadOptionAccess("Cannot unwrap an Option that is None.");
}

/*********************************************************************
** Function: operator=
** Description: Overloads the assignment operator for implement safe copying
 * of Option classes.
** Parameters: rhs is the Option<T> being copied.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
template <class T>
Option<T> &Option<T>::operator=(const Option<T> &rhs) {
  if (IsSome())
    value_.~T();

  has_value_ = rhs.has_value_;
  moved_ = rhs.moved_;

  if (rhs.IsSome())
    new (&value_) T(rhs.value_);
  else
    stub_ = nullptr;

  return *this;
}

/*********************************************************************
** Function: operator<<
** Description: Overloads the insertion operator to print Option<T> values.
** Parameters: os is the output_stream; possible_t is the Option<T> to print.
** Pre-Conditions:  None
** Post-Conditions: None
*********************************************************************/
template <typename T>
std::ostream &operator<<(std::ostream &os, const Option<T> &possible_t) {
  if (possible_t.IsNone())
    return os << "None";

  return os << "Some(" << possible_t.CUnwrapRef() << ')';
}


#endif //ZOO_TYCOON_OPTION_H
