#include "Client.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor
ClientNode::ClientNode(const string& username, const string& password, int ssn, const string& name, int role)
    : username(username), password(password), ssn(ssn), name(name), role(role), accountHead(nullptr), next(nullptr) {}

// Destructor
ClientNode::~ClientNode() {
    AccountNode* currentAccount = accountHead;
    while (currentAccount) {
        AccountNode* tempAccount = currentAccount;
        currentAccount = currentAccount->next;
        delete tempAccount;
    }
}

// Add account
void ClientNode::addAccount(AccountNode* account) {
    account->next = accountHead;
    accountHead = account;
}

// Delete account
bool ClientNode::deleteAccount(const string& accountNumber) {
    AccountNode* current = accountHead;
    AccountNode* previous = nullptr;

    while (current && current->accountNumber != accountNumber) {
        previous = current;
        current = current->next;
    }

    if (current) {
        if (previous) {
            previous->next = current->next;
        } else {
            accountHead = current->next;
        }
        delete current;
        return true;
    }
    return false;
}

// Find account
AccountNode* ClientNode::findAccount(const string& accountNumber) {
    AccountNode* current = accountHead;
    while (current) {
        if (current->accountNumber == accountNumber) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void ClientNode::displayAccounts() const {
    if (!accountHead) {
        cout << "\033[1;31mNo accounts available.\033[0m\n";
        return;
    }

    cout << "\033[1;35m";  // Magenta text for table headers
    cout << "╔═════════════════════╦═════════════════════════════════╦═════════════════╗" << "\n";
    cout << "║" << setw(20) << "Account Number" << " ║ "
         << setw(31) << "Account Type" << " ║ "
         << setw(15) << "Balance" << " ║\n";
    cout << "╠═════════════════════╬═════════════════════════════════╬═════════════════╣" << "\n";
    cout << "\033[0m";  // Reset text

    AccountNode* account = accountHead;
    while (account) {
        cout << "\033[1;35m║\033[0m" << setw(20) << account->accountNumber << "\033[1;35m ║ \033[0m"
             << setw(31) << account->accountType;
        cout << "\033[1;35m ║ \033[0m" << setw(15) << fixed << setprecision(2) << account->balance << "\033[1;35m ║\033[0m";
		  if (account->accountType == "CertificateOfDeposit") {
            cout << " Maturity: " << account->maturityMonths << " months\033[1;35m ║\033[0m";
        }
		  cout << endl;

        account = account->next;
    }
    cout << "\033[1;35m"
         << "╚═════════════════════╩═════════════════════════════════╩═════════════════╝"
         << "\033[0m\n";
}
