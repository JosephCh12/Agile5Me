#ifndef BANK_H
#define BANK_H

#include "Client.h"
#include "Utils.h"
#include <string>
#include <limits>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

class Bank {
private:
    ClientNode* clientHead;  // Head of the linked list of clients
    int nextAccountNumber;   // For sequential account number generation

public:
    // Constructor
    Bank();

    // Destructor
    ~Bank();

    // Initialize account number generator
    void initializeAccountNumberGenerator();

    // Load clients and accounts from a file
    bool loadFromFile(const std::string& filename);

    // Save clients and accounts to a file
    bool saveToFile(const std::string& filename);

    // Display the starting page
    void displayStartingPage();

    // Display help information
    void displayHelp();

    // Client interface
    void clientInterface(ClientNode* client, std::string fileName);

    // Employee interface
    void employeeInterface(ClientNode* client, std::string fileName);

    // Helper functions
    ClientNode* findClientByUsername(const std::string& username);
    ClientNode* createNewClient(ClientNode* current, std::string fileName);
    ClientNode* findClientBySSN(int ssn);

    // Generate unique account number
    std::string generateAccountNumber();

    // Employee functions
    void employeeMenu(ClientNode* employee, std::string fileName);
    void handleViewAllClients();
    void handleAddClient(ClientNode* employee, std::string fileName);
    void handleDeleteClient(ClientNode* employee, std::string fileName);
    void handleAddAccountToClient(ClientNode* employee, std::string fileName, ClientNode* client2 = nullptr);
    void handleDeleteAccountFromClient(ClientNode* employee, std::string fileName);
    void handleDepositToAccount(ClientNode* employee, std::string fileName);
    void handleWithdrawFromAccount(ClientNode* employee, std::string fileName);
    void handleViewClientAccounts();

    // Client functions
    void clientMenu(ClientNode* client, std::string fileName);
    void handleClientViewAccounts(ClientNode* client);
    void handleClientAddAccount(ClientNode* client, std::string fileName);
    void handleClientDeposit(ClientNode* client, std::string fileName);
    void handleClientWithdraw(ClientNode* client, std::string fileName);

	int getch();
	std::string GetHiddenPassword();
	void SavePersonUsingData(ClientNode *head);
	ClientNode* LoginSystem();

};

#endif // BANK_H
