#ifndef CLIENT_H
#define CLIENT_H

#include "Account.h"
#include <string>

struct ClientNode {
    std::string username;
    std::string password;
    int ssn;
    std::string name;
    int role;
    AccountNode* accountHead;
    ClientNode* next;

    // Constructor
    ClientNode(const std::string& username, const std::string& password, int ssn, const std::string& name, int role = 0);

    // Destructor
    ~ClientNode();

    // Account management
    void addAccount(AccountNode* account);
    bool deleteAccount(const std::string& accountNumber);
    AccountNode* findAccount(const std::string& accountNumber);

    // Display accounts
    void displayAccounts() const;
};

#endif // CLIENT_H
