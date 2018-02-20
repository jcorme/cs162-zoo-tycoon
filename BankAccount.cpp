/*********************************************************************
** Program Filename: BankAccount.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Implements functions declared by the BankAccount class
 * and in the BankAccount header.
** Input: None
** Output: None
*********************************************************************/
#include <iostream>
#include "BankAccount.h"

/*********************************************************************
** Function: Deposit
** Description: Deposits the given amount into the bank account.
** Parameters: amount is the amount to deposit; reason is a description
 * of the deposit.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
void BankAccount::Deposit(double amount, const std::string &reason) {
  balance_ += amount;
  BankAccountTransaction t = BankAccountTransaction(
      BankTransactionType::Deposit,
      amount,
      reason
  );
  LogTransaction(t);
}

/*********************************************************************
** Function: LogTransaction
** Description: Adds the transaction to the account's record.
** Parameters: t is the transaction to record.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
void BankAccount::LogTransaction(BankAccountTransaction t) {
  transactions_.push_back(t);
}

/*********************************************************************
** Function: Withdraw
** Description: Removes the specified amount from the bank account.
** Parameters: amount is the amount to remove; reason is a description
 * of the withdrawal; returns false if the account has insufficient
 * funds.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
bool BankAccount::Withdraw(double amount, const std::string &reason) {
  if (!CanAfford(amount)) return false;

  balance_ -= amount;
  BankAccountTransaction t = BankAccountTransaction(
      BankTransactionType::Withdrawal,
      amount,
      reason
  );
  LogTransaction(t);
  return true;
}

/*********************************************************************
** Function: operator<<
** Description: Overloads the insertion operator to print out account
 * information.
** Parameters: os is an output stream; b is the account to print.
** Pre-Conditions: None
** Post-Conditions: None
*********************************************************************/
std::ostream &operator<<(std::ostream &os, const BankAccount &b) {
  os << "Current Balance: $" << b.balance_ << '\n'
     << "Number of Transactions: " << b.transactions_.size() << '\n'
     << "Transactions: " << '\n';

  auto sz = b.transactions_.size();
  for (decltype(sz) i = 0; i != sz; ++i) {
    os << '\t' << i + 1 << ") $" << b.transactions_[i].amount() << ": "
       << b.transactions_[i].description();

    if (i != sz - 1) os << '\n';
  }

  return os;
}
