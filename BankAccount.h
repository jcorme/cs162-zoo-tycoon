#ifndef ZOO_TYCOON_BANKACCOUNT_H
#define ZOO_TYCOON_BANKACCOUNT_H


#include <string>
#include <vector>

enum class TransactionType
{
  Deposit,
  Withdrawal
};

struct BankAccountTransaction
{
  BankAccountTransaction(TransactionType type, double amount,
      const std::string &desc);

  TransactionType type;
  double amount;
  std::string description;
};

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
    bool Withdraw(double amount, const std::string &reason);

    void LogTransaction(BankAccountTransaction t);

  private:
    double balance_;

    std::vector<BankAccountTransaction> transactions_;
};

std::ostream &operator<<(std::ostream &os, const BankAccount &b);


#endif //ZOO_TYCOON_BANKACCOUNT_H
