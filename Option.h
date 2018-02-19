#ifndef ZOO_TYCOON_OPTION_H
#define ZOO_TYCOON_OPTION_H


#include <functional>
#include <iostream>
#include <utility>
#include <exception>

class BadOptionAccess: public std::exception {
  public:
    explicit BadOptionAccess(const char *msg): msg_(msg) {}
    const char *what() const noexcept override { return msg_; }

  private:
    const char *msg_;
};

struct None {};
constexpr None None{};

template <class T>
class Option {
  public:
    Option(): has_value_(false), moved_(false), stub_(nullptr) {}
    Option(const T &t): has_value_(true), moved_(false), value_(t) {}
    Option(T &&t): has_value_(true), moved_(false), value_(std::move(t)) {}
    Option(const Option<T> &t);
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
    Option<E> AndThen(std::function<Option<E>(T)> fn);

    template <class E>
    Option<E> Map(std::function<E(T)> fn);

    template <class E>
    Option<E> MapCRef(std::function<E(const T &)> fn) const;

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

template <class T>
Option<T>::Option(const Option<T> &t) {
  *this = t;
}

template <class T>
Option<T>::~Option() {
  if (IsSome())
    value_.~T();
}

template <class T>
const T &Option<T>::CUnwrapRef() const {
  ThrowIfBad();
  return value_;
}

template <class T>
const T &Option<T>::CUnwrapRefOr(const T &t) const {
  if (IsNone()) return t;
  return CUnwrapRef();
}

template <class T>
T Option<T>::Unwrap() {
  ThrowIfBad();
  moved_ = true;
  return std::move(value_);
}

template <class T>
T Option<T>::UnwrapOr(T t)
{
  if (IsNone()) return t;
  return Unwrap();
}

template <class T>
T &Option<T>::UnwrapRef() const {
  ThrowIfBad();
  // I'm not entirely sure why g++ 6.3.0 qualifies value_ with const.
  // This probably isn't the safest thing in the world...
  return const_cast<T &>(value_);
}

template <class T>
template <class E>
Option<E> Option<T>::AndThen(std::function<Option<E>(T)> fn) {
  if (IsNone()) return None;
  moved_ = true;
  return fn(std::move(value_));
}

template <class T>
template <class E>
Option<E> Option<T>::Map(std::function<E(T)> fn) {
  if (IsNone()) return None;
  moved_ = true;
  return fn(std::move(value_));
};

template <class T>
template <class E>
Option<E> Option<T>::MapCRef(std::function<E(const T &)> fn) const {
  if (IsNone()) return None;
  return fn(value_);
};

template <class T>
void Option<T>::ThrowIfBad() const {
  if (moved_)
    throw BadOptionAccess("Cannot unwrap moved value.");

  if (!has_value_)
    throw BadOptionAccess("Cannot unwrap an Option that is None.");
}

template <class T>
Option<T> &Option<T>::operator=(const Option<T> &rhs) {
  has_value_ = rhs.has_value_;
  moved_ = rhs.moved_;

  if (IsSome())
    value_.~T();

  if (rhs.IsSome())
    value_ = rhs.value_;
  else
    stub_ = nullptr;

  return *this;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Option<T> &possible_t) {
  if (possible_t.IsNone())
    return os << "None";

  return os << "Some(" << possible_t.CUnwrapRef() << ')';
}


#endif //ZOO_TYCOON_OPTION_H
