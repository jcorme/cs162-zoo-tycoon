#ifndef ZOOTYCOON_BANKACCOUNTTRANSACTION_H
#define ZOOTYCOON_BANKACCOUNTTRANSACTION_H
/*********************************************************************
** Program Filename: BankAccountTransaction.h
** Author: Jason Chen
** Date: 02/19/2018
** Description: Declares the BankAccountTransaction class and its members. 
** Input: None
** Output: None
*********************************************************************/


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
        amount_(amount), description_(desc), type_(type) {}

    double amount() const { return amount_; }
    const std::string &description() const { return description_; }
    BankTransactionType type() const { return type_; }

  private:
    double amount_;
    std::string description_;
    BankTransactionType type_;
};


#endif //ZOOTYCOON_BANKACCOUNTTRANSACTION_H
