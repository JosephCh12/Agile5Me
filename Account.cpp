#include "Account.h"

// Constructor
AccountNode::AccountNode(const std::string& accountNumber, const std::string& accountType, double balance, int maturityMonths)
    : accountNumber(accountNumber), accountType(accountType), balance(balance), maturityMonths(maturityMonths), next(nullptr) {}

// Destructor
AccountNode::~AccountNode() {
    // No dynamic memory to free in this class
}

// Deposit
void AccountNode::deposit(double amount) {
    balance += amount;
}

// Withdraw
void AccountNode::withdraw(double amount) {
    balance -= amount;
}
