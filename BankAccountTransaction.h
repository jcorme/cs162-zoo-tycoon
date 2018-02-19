#ifndef ZOOTYCOON_BANKACCOUNTTRANSACTION_H
#define ZOOTYCOON_BANKACCOUNTTRANSACTION_H


#include <string>

enum class BankTransactionType {
  Deposit,
  Withdrawal
};

class BankAccountTransaction
{
  public:
    BankAccountTransaction(
        BankTransactionType type, double amount, const std::string &desc):
        type_(type), amount_(amount), description_(desc) {}

    double amount() const { return amount_; }
    const std::string &description() const { return description_; }
    BankTransactionType type() const { return type_; }

  private:
    double amount_;
    std::string description_;
    BankTransactionType type_;
};


#endif //ZOOTYCOON_BANKACCOUNTTRANSACTION_H
