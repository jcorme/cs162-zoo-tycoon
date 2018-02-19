#ifndef ZOO_TYCOON_BANKACCOUNT_H
#define ZOO_TYCOON_BANKACCOUNT_H


#include <string>
#include <vector>
#include "BankAccountTransaction.h"

class BankAccount
{
  friend std::ostream &operator<<(std::ostream &os, const BankAccount &b);

  public:
    explicit BankAccount(double balance = 0.0): balance_(balance) {}

    double balance() const { return balance_; }
    std::vector<BankAccountTransaction> transactions() const
        { return transactions_; }

    bool CanAfford(double amount) const { return amount <= balance_; };

    void Deposit(double amount, const std::string &reason);
    void LogTransaction(BankAccountTransaction t);
    bool Withdraw(double amount, const std::string &reason);

  private:
    double balance_;

    std::vector<BankAccountTransaction> transactions_;
};

std::ostream &operator<<(std::ostream &os, const BankAccount &b);


#endif //ZOO_TYCOON_BANKACCOUNT_H
