#include <iostream>
#include "BankAccount.h"

BankAccountTransaction::BankAccountTransaction(
    TransactionType type, double amount, const std::string &desc):
    type(type), amount(amount), description(desc) {}

void BankAccount::Deposit(double amount, const std::string &reason)
{
  balance_ += amount;
  BankAccountTransaction t = BankAccountTransaction(
      TransactionType::Deposit,
      amount,
      reason
  );
  LogTransaction(t);
}

bool BankAccount::Withdraw(double amount, const std::string &reason)
{
  if (!CanAfford(amount)) return false;

  balance_ -= amount;
  BankAccountTransaction t = BankAccountTransaction(
      TransactionType::Withdrawal,
      amount,
      reason
  );
  LogTransaction(t);
  return true;
}

void BankAccount::LogTransaction(BankAccountTransaction t)
{
  transactions_.push_back(t);
}

std::ostream &operator<<(std::ostream &os, const BankAccount &b)
{
  os << "Current Balance: $" << b.balance_ << '\n'
     << "Number of Transactions: " << b.transactions_.size() << '\n'
     << "Transactions: " << '\n';

  auto sz = b.transactions_.size();
  for (decltype(sz) i = 0; i != sz; ++i)
  {
    os << '\t' << i + 1 << ") $" << b.transactions_[i].amount << ": "
       << b.transactions_[i].description;

    if (i != sz - 1)
      os << '\n';
  }

  return os;
}
