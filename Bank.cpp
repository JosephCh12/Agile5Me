#include "Bank.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

// Constructor
Bank::Bank() : clientHead(nullptr), nextAccountNumber(100000) {}

// Destructor
Bank::~Bank() {
    // Free the linked list of clients and their accounts
    ClientNode* currentClient = clientHead;
    while (currentClient) {
        ClientNode* tempClient = currentClient;
        currentClient = currentClient->next;
        delete tempClient;
    }
}

// Initialize account number generator
void Bank::initializeAccountNumberGenerator() {
    // Set nextAccountNumber based on existing accounts to ensure uniqueness
    ClientNode* client = clientHead;
    while (client) {
        AccountNode* account = client->accountHead;
        while (account) {
            int accNum = stoi(account->accountNumber);
            if (accNum >= nextAccountNumber) {
                nextAccountNumber = accNum + 1;
            }
            account = account->next;
        }
        client = client->next;
    }
}

// Helper function to convert string to lowercase
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

bool Bank::loadFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "\033[1;31mError opening file!\033[0m" << std::endl;
        return false;
    }

    std::string line;
    while (getline(infile, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        line = DataDecryption(line);

        std::istringstream iss(line);
        std::string username, password, name;
        int ssn, role;

        if (!(iss >> username >> password >> ssn >> role)) {
            std::cerr << "\033[1;31mError: Malformed input in file!\033[0m" << std::endl;
            continue;
        }

        if (line.size() > static_cast<std::string::size_type>(iss.tellg()) && iss.tellg() != -1) {
            getline(iss, name);
            if (!name.empty()) {
                name = name.substr(1); // Remove leading space if name is valid
            } else {
                std::cerr << "\033[1;31mError: Name is empty!\033[0m" << std::endl;
                continue;
            }
        } else {
            std::cerr << "\033[1;31mError: No name found in file!\033[0m" << std::endl;
            continue;
        }

        ClientNode* client = new ClientNode(username, password, ssn, name, role);

        std::string numAcct;
        int numAccounts;
        if (!(infile >> numAcct)) {
            std::cerr << "\033[1;31mError: Failed to read number of accounts!\033[0m" << std::endl;
            continue;
        }
        numAcct = DataDecryption(numAcct);
        numAccounts = std::stoi(numAcct);

        infile.ignore();

        for (int i = 0; i < numAccounts; ++i) {
            std::string accountLine;
            if (!getline(infile, accountLine)) {
                std::cerr << "\033[1;31mError: Account entry missing!\033[0m" << std::endl;
                break;
            }
            accountLine = DataDecryption(accountLine);

            std::istringstream accountStream(accountLine);
            std::string accountNumber, accountType;
            double balance;
            int maturityMonths = 0;

            if (!(accountStream >> accountNumber >> accountType >> balance)) {
                std::cerr << "\033[1;31mError: Malformed account data!\033[0m" << std::endl;
                continue;
            }

            if (accountType == "CertificateOfDeposit") {
                accountStream >> maturityMonths;
            }

            AccountNode* account = new AccountNode(accountNumber, accountType, balance, maturityMonths);
            client->addAccount(account);
        }

        client->next = clientHead;
        clientHead = client;

        getline(infile, line);
    }

    infile.close();
    return true;
}


// Save clients and accounts to a file
bool Bank::saveToFile(const string& filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "\033[1;31mError opening file!\033[0m" << endl;
        return false;
    }

    ClientNode* client = clientHead;
    while (client) {
        std::string ssn, role, line;
        ssn = std::to_string(client->ssn);
        role = std::to_string(client->role);

        line = client->username + " " + client->password + " " +
               ssn + " " + role + " " + client->name;
        line = DataEncryption(line);

        outfile << line << endl;

        // Count number of accounts
        int numAccounts = 0;
        AccountNode* account = client->accountHead;
        while (account) {
            ++numAccounts;
            account = account->next;
        }
        std::string numAcct = std::to_string(numAccounts);
        numAcct = DataEncryption(numAcct);
        outfile << numAcct << endl;

        account = client->accountHead;
        while (account) {
            std::string balance = std::to_string(account->balance);
            std::string line2 = account->accountNumber + " " + account->accountType + " " + balance;

            // Include maturity months if it's a Certificate of Deposit
            if (account->accountType == "CertificateOfDeposit") {
                line2 += " " + std::to_string(account->maturityMonths);
            }

            line2 = DataEncryption(line2);
            outfile << line2 << endl;

            account = account->next;
        }

        outfile << endl; // Separate clients with an empty line
        client = client->next;
    }

    outfile.close();
    return true;
}


// Display the starting page
void Bank::displayStartingPage() {
    clearScreen();

    // Using UTF-8 encoded string literals
    const string borderMiddle = "╠═════════════════════════════════════════════════════════════════════════╣\n";
    const string borderBottom = "╚═════════════════════════════════════════════════════════════════════════╝\n";

    displayMenuHeader("Welcome to the Bank System");
    cout << "\033[1;34m";  // Bold Blue text
    cout << borderMiddle;
    cout << "║" << centerText("1. Client", 72)   << " ║\n";
    cout << "║" << centerText("2. Employee", 73) << "║\n";
    cout << "║" << centerText("3. Help", 70)     << "   ║\n";
    cout << "║" << centerText("4. Exit", 70)     << "   ║\n";
    cout << borderBottom;
    cout << "\033[0m";  // Reset text
}

// Display help information
void Bank::displayHelp() {
    clearScreen();
    cout << "\033[1;34m" << centerText("Help Information") << "\033[0m\n\n";
    cout << "\033[1;35m" << centerText("This is a console-based banking system where you can:") << endl;
    cout << centerText("Clients can manage their accounts, perform deposits and withdrawals.") << endl;
    cout << centerText("Employees can manage clients and their accounts.") << endl;
    cout << centerText("Please follow the on-screen prompts and enter the required information.") << endl;
    cout << "\033[0m";
    cout << "\nPress Enter to return to the main menu...";
    cin.get();
}

void Bank::clientInterface(ClientNode* client, string fileName) {
    clearScreen();
    clientMenu(client, fileName);

}

// Employee interface
void Bank::employeeInterface(ClientNode* client, string fileName) {
	clearScreen();
    employeeMenu(client, fileName);
}

// Helper function to find a client by username (case-insensitive)
ClientNode* Bank::findClientByUsername(const string& username) {
    ClientNode* current = clientHead;
    string lowerUsername = toLowerCase(username);
    while (current) {
        if (toLowerCase(current->username) == lowerUsername) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Helper function to find a client by SSN
ClientNode* Bank::findClientBySSN(int ssn) {
    ClientNode* current = clientHead;
    while (current) {
        if (current->ssn == ssn) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Create a new client
ClientNode* Bank::createNewClient(ClientNode* current, string fileName) {
    const string borderTop    = "╔═════════════════════════════════════════════════════════════════════════╗\n";
    const string borderBottom = "╚═════════════════════════════════════════════════════════════════════════╝\n";
    clearScreen();
    string username, password, name;
    int ssn;

    displayMenuHeader("Create New Login Account");

    // Username input and validation
    while (true) {
        cout << "\033[1;35mEnter Username: \033[0m";  // Purple for input prompt
        getline(cin, username);
        if (findClientByUsername(username)) {
            cout << "\033[1;31mExisting username try again.\033[0m\n";
        } else {
            break;
        }
    }

    // Password input
    cout << "\033[1;35mEnter Password: \033[0m";  // Purple for input prompt
    password = GetHiddenPassword();
	 cout << endl;

    // SSN input and validation
    while (true) {
        cout << "\033[1;35mEnter SSN (numbers only): \033[0m";  // Purple for input prompt
        string ssnInput;
        getline(cin, ssnInput);
        if (!isNumber(ssnInput) || ssnInput.empty()) {
            cout << "\033[1;31mInvalid SSN. Please enter numbers only.\033[0m\n";
        } else {
            ssn = stoi(ssnInput);
            if (findClientBySSN(ssn)) {
                cout << "\033[1;31mA client with this SSN already exists.\033[0m\n";
            } else {
                break;
            }
        }
	 }

    // Name input
    cout << "\033[1;35mEnter Full Name: \033[0m";  // Purple for input prompt
    getline(cin, name);

	int role = 0;

	if(current->role == 1)
	{
		cout << "\n\033[1;34m";
		cout << borderTop;
		cout << "║" << centerText("Select Login Account Type:", 73)      << "║\n";
		cout << "║" <<  centerText("1. Client", 73)              << "║\n";
		cout << "║" << centerText("2. Employee", 73)                << "║\n";
		cout << borderBottom << endl;
		cout << "\033[0m\n";
		role = getValidatedChoice(1, 2);

		switch (role) {
			case 1:
				role -= 1;
				break;
			case 2:
				role -= 1;
				break;
			default:
				cout << "\033[1;31mInvalid input.\033[0m\n";
			pauseForUser();
		}
	}
	else
	{
		cout << "\n\033[1;34m";
		cout << borderTop;
		cout << "║" << centerText("Select Login Account Type:", 73)      << "║\n";
		cout << "║" <<  centerText("1. Client", 73)              << "║\n";
		cout << "║" << centerText("2. Employee", 73)                << "║\n";
		cout << "║" << centerText("3. Manager", 73)  << "║\n";
		cout << borderBottom << endl;
		cout << "\033[0m\n";
		role = getValidatedChoice(1, 3);

		switch (role) {
			case 1:
				role -= 1;
				break;
			case 2:
				role -= 1;
				break;
			case 3:
				role -= 1;
				break;
			default:
				cout << "\033[1;31mInvalid input.\033[0m\n";
			pauseForUser();
		}
	}

    ClientNode* newClient = new ClientNode(username, password, ssn, name, role);
    // Insert new client at the beginning of the list
    newClient->next = clientHead;
    clientHead = newClient;
    cout << "\033[1;32mClient account created successfully!\033[0m\n";

	 if(!saveToFile(fileName))
	 {
		cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
	 }

    return newClient;
}

// Generate unique account number
string Bank::generateAccountNumber() {
    return to_string(nextAccountNumber++);
}

// Employee menu
void Bank::employeeMenu(ClientNode* employee, string fileName) {
    bool backToMainMenu = false;
    while (!backToMainMenu) {
        clearScreen();
		  string header;

		  switch(employee->role)
		  {
				case 1:
					header = employee->name + ": Employee Menu";
					break;
				default:
					header = employee->name + ": Manager Menu";
		  }
        displayMenuHeader(header);

        const string borderMiddle = "╠═════════════════════════════════════════════════════════════════════════╣\n";
        const string borderBottom = "╚═════════════════════════════════════════════════════════════════════════╝\n";

        cout << "\033[1;34m";  // Bold Blue text
        cout << borderMiddle;
        cout << "║" << centerText("1. View All Clients", 73)           << "║\n";
        cout << "║" << centerText("2. Add New Client", 73)             << "║\n";
        cout << "║" << centerText("3. Delete Client", 73)              << "║\n";
        cout << "║" << centerText("4. Add Account to Client", 73)      << "║\n";
        cout << "║" << centerText("5. Delete Account from Client", 73) << "║\n";
        cout << "║" << centerText("6. Deposit to Account", 73)         << "║\n";
        cout << "║" << centerText("7. Withdraw from Account", 73)      << "║\n";
        cout << "║" << centerText("8. View Client's Accounts", 73)     << "║\n";
        cout << "║" << centerText("9. Back to Main Menu", 73)          << "║\n";
        cout << borderBottom;
        cout << "\033[0m\n";  // Reset text

        int choice = getValidatedChoice(1, 9);

        switch (choice) {
            case 1:
                handleViewAllClients();
                break;
            case 2:
                handleAddClient(employee, fileName);
                break;
            case 3:
                handleDeleteClient(employee, fileName);
                break;
            case 4:
                handleAddAccountToClient(employee, fileName, nullptr);
                break;
            case 5:
                handleDeleteAccountFromClient(employee, fileName);
                break;
            case 6:
                handleDepositToAccount(employee, fileName);
                break;
            case 7:
                handleWithdrawFromAccount(employee, fileName);
                break;
            case 8:
                handleViewClientAccounts();
                break;
            case 9:
                backToMainMenu = true;
                logAction(employee->username, "Logout", "Employee logged out.");
                break;
            default:
                cout << "\033[1;31mInvalid choice. Please try again.\033[0m\n";
                pauseForUser();
        }

        // Add 4 empty lines between menus
        std::cout << "\n\n\n\n";
    }
}

// Client Menu
void Bank::clientMenu(ClientNode* client, string fileName) {
    bool logout = false;
    while (!logout) {
        clearScreen();
		  string header = client->name + ": Client Menu";
        displayMenuHeader(header);

        const string borderMiddle = "╠═════════════════════════════════════════════════════════════════════════╣\n";
        const string borderBottom = "╚═════════════════════════════════════════════════════════════════════════╝\n";
        cout << "\033[1;34m";  // Bold Blue text
        cout << borderMiddle;
        cout << "║" << centerText("1. View Accounts", 73) << "║\n";
        cout << "║" << centerText("2. Add Account", 73) << "║\n";
        cout << "║" << centerText("3. Deposit", 73) << "║\n";
        cout << "║" << centerText("4. Withdraw", 73) << "║\n";
        cout << "║" << centerText("5. Logout", 73) << "║\n";
        cout << borderBottom;
        cout << "\033[0m\n";  // Reset text

        int choice = getValidatedChoice(1, 5);

        switch (choice) {
            case 1:
                handleClientViewAccounts(client);
                break;
            case 2:
                handleClientAddAccount(client, fileName);
                break;
            case 3:
                handleClientDeposit(client, fileName);
                break;
            case 4:
                handleClientWithdraw(client, fileName);
                break;
            case 5:
                logout = true;
                logAction(client->username, "Logout", "User logged out.");
                break;
            default:
                cout << "\033[1;31mInvalid choice. Please try again.\033[0m\n";
                pauseForUser();
        }

        // Add 4 empty lines between menus
        std::cout << "\n\n\n\n";
    }
}

// Employee Functions

void Bank::handleViewAllClients() {
    clearScreen();
    displayMenuHeader("All Clients");
	 cout << endl;

    ClientNode* client = clientHead;
    if (!client) {
        cout << "\033[1;31mNo clients available.\033[0m\n";
        pauseForUser();
        return;
    }
    cout << "\033[1;35m";
    cout << setw(4)  << ' '
			<< setw(20) << "Username" << " | "
         << setw(30) << "Name" << " | "
         << setw(10) << "SSN" << endl;
    cout << setw(4) << ' ' << string(67, '-') << "\n";
    cout << "\033[0m";
    while (client) {
        cout << setw(4)  << ' '
				 << setw(20) << client->username << "\033[1;35m | \033[0m"
             << setw(30) << client->name << "\033[1;35m | \033[0m"
             << setw(10) << client->ssn << endl;
        client = client->next;
    }
	 cout << setw(4)  << ' '
		   << setw(20) << ' ' << "\033[1;35m | \033[0m"
         << setw(30) << ' ' << "\033[1;35m | \033[0m\n";

    pauseForUser();
}

void Bank::handleAddClient(ClientNode* employee, string fileName){
        string name;
    	int ssn;
    	//deadlocking
    	if(!ReadDeadlockBool(name, ssn))
    	{
    	SavePersonUsingData(employee);

    	createNewClient(employee, fileName);

    	FreeDeadlock();
    	}
    	else
    	{
    		cout << "File is being use by " << name << ", ssn: " << ssn << endl;
    		cin.ignore(1000, '\n');
    	}
}

void Bank::handleDeleteClient(ClientNode* employee, string fileName) {
	string name;
	int ssn;
	//deadlocking
	if(!ReadDeadlockBool(name, ssn))
	{
	SavePersonUsingData(employee);

    string username;
    cout << "Enter the username of the client to delete: ";
    getline(cin, username);

    ClientNode* current = clientHead;
    ClientNode* previous = nullptr;

    while (current && toLowerCase(current->username) != toLowerCase(username)) {
        previous = current;
        current = current->next;
    }

    if (current) {
        // Delete client's accounts
        AccountNode* account = current->accountHead;
        while (account) {
            AccountNode* tempAccount = account;
            account = account->next;
            delete tempAccount;
        }

        // Remove client from linked list
        if (previous) {
            previous->next = current->next;
        } else {
            clientHead = current->next;
        }

        delete current;
        cout << "\033[1;32mClient deleted successfully.\033[0m\n";

			 if(!saveToFile(fileName))
			 {
				cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
			 }

    } else {
        cout << "\033[1;31mClient not found.\033[0m\n";
    }
    pauseForUser();

    FreeDeadlock();
	}
	else
	{
		cout << "File is being use by " << name << ", ssn: " << ssn << endl;
		cin.ignore(1000, '\n');
	}
}

void Bank::handleAddAccountToClient(ClientNode* employee, string fileName, ClientNode* client2) {
    string name;
    int ssn;

    // Deadlocking check
    if(!ReadDeadlockBool(name, ssn)) {
        SavePersonUsingData(employee);

        clearScreen();
        displayMenuHeader("Add An Account");

        if (!client2) {
            cout << "Enter the client's username: ";
            string username;
            getline(cin, username);

            client2 = findClientByUsername(username);
            if (!client2) {
                cout << "\033[1;31mClient not found.\033[0m\n";
                pauseForUser();
                return;
            }
        }

        // Display account type options
        const string borderTop    = "╔═════════════════════════════════════════════════════════════════════════╗";
        const string borderBottom = "╚═════════════════════════════════════════════════════════════════════════╝\n";

        string accountNumber = generateAccountNumber();
        string accountType;
        double balance;
        int maturityMonths = 0;

        cout << "\033[1;34m";
        cout << borderTop << endl;
        cout << "║" << centerText("Select Account Type:", 73) << "║\n";
        cout << "║" << centerText("1. Savings Account", 73) << "║\n";
        cout << "║" << centerText("2. High Interest Savings", 73) << "║\n";
        cout << "║" << centerText("3. No Service Charge Checking", 73) << "║\n";
        cout << "║" << centerText("4. Service Charge Checking", 73) << "║\n";
        cout << "║" << centerText("5. Higher Interest Checking", 73) << "║\n";
        cout << "║" << centerText("6. Certificate of Deposit", 73) << "║\n";
        cout << borderBottom;
        cout << "\033[0m\n";

        cout << "Generated Account Number: " << accountNumber << endl;

        int typeChoice = getValidatedChoice(1, 6);

        switch (typeChoice) {
            case 1:
                accountType = "SavingsAccount";
                break;
            case 2:
                accountType = "HighInterestSavings";
                break;
            case 3:
                accountType = "NoServiceChargeChecking";
                break;
            case 4:
                accountType = "ServiceChargeChecking";
                break;
            case 5:
                accountType = "HigherInterestChecking";
                break;
            case 6:
                accountType = "CertificateOfDeposit";
                cout << "Enter maturity period in months: ";
                while (!(cin >> maturityMonths) || maturityMonths <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\033[1;31mInvalid input. Please enter a positive number for maturity months.\033[0m\n";
                    cout << "Enter maturity period in months: ";
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            default:
                cout << "\033[1;31mInvalid account type.\033[0m\n";
                pauseForUser();
                return;
        }

        cout << "Enter Initial Balance: ";
        while (!(cin >> balance) || balance < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[1;31mInvalid amount. Please enter a non-negative number.\033[0m\n";
            cout << "Enter Initial Balance: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        AccountNode* newAccount = new AccountNode(accountNumber, accountType, balance, maturityMonths);
        client2->addAccount(newAccount);
        cout << "\033[1;32mAccount added successfully.\033[0m\n";

        if (!saveToFile(fileName)) {
            cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
        }

        pauseForUser();

        FreeDeadlock();
    } else {
        cout << "File is being used by " << name << ", ssn: " << ssn << endl;
        cin.ignore(1000, '\n');
    }
}


void Bank::handleDeleteAccountFromClient(ClientNode* employee, string fileName) {
	string name;
	int ssn;
	//deadlocking
	if(!ReadDeadlockBool(name, ssn))
	{
	SavePersonUsingData(employee);

	string username;
    cout << "Enter the client's username: ";
    getline(cin, username);

    ClientNode* client = findClientByUsername(username);
    if (client) {
        client->displayAccounts();

        string accountNumber;
        cout << "Enter the account number to delete: ";
        getline(cin, accountNumber);

        if (client->deleteAccount(accountNumber)) {
            cout << "\033[1;32mAccount deleted successfully.\033[0m\n";

				if(!saveToFile(fileName))
				 {
					cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
				 }

        } else {
            cout << "\033[1;31mAccount not found.\033[0m\n";
        }
    } else {
        cout << "\033[1;31mClient not found.\033[0m\n";
    }
    pauseForUser();

    FreeDeadlock();
	}
	else
	{
		cout << "File is being use by " << name << ", ssn: " << ssn << endl;
		cin.ignore(1000, '\n');
	}
}

void Bank::handleDepositToAccount(ClientNode* employee, string fileName) {
	string name;
	int ssn;
	//deadlocking
	if(!ReadDeadlockBool(name, ssn))
	{
	SavePersonUsingData(employee);

	string username;
	clearScreen();
    displayMenuHeader("Deposit To Account");
	cout << endl;
    cout << "Enter the client's username: ";
    getline(cin, username);
	 cout << endl;

    ClientNode* client = findClientByUsername(username);
    if (client) {
        client->displayAccounts();

        string accountNumber;
        cout << "Enter the account number to deposit into: ";
        getline(cin, accountNumber);

        AccountNode* account = client->findAccount(accountNumber);
        if (account) {
            double amount;
            cout << "Enter the amount to deposit: ";
            while (!(cin >> amount) || amount <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\033[1;31mInvalid amount. Please enter a positive number.\033[0m\n";
                cout << "Enter the amount to deposit: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            account->deposit(amount);
            logAction(username, "Deposit", "Deposited " + to_string(amount) + " to account " + accountNumber);
            cout << "\033[1;32mDeposit successful.\033[0m\n";

				if(!saveToFile(fileName))
				 {
					cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
				 }

        } else {
            cout << "\033[1;31mAccount not found.\033[0m\n";
        }
    } else {
        cout << "\033[1;31mClient not found.\033[0m\n";
    }
    pauseForUser();

    FreeDeadlock();
	}
	else
	{
		cout << "File is being use by " << name << ", ssn: " << ssn << endl;
		cin.ignore(1000, '\n');
	}
}

void Bank::handleWithdrawFromAccount(ClientNode* employee, string fileName) {
	string name;
	int ssn;
	//deadlocking
	if(!ReadDeadlockBool(name, ssn))
	{
	SavePersonUsingData(employee);

	string username;
	clearScreen();
    displayMenuHeader("Withdraw From Account");
	cout << endl;
    cout << "Enter the client's username: ";
    getline(cin, username);
	 cout << endl;

    ClientNode* client = findClientByUsername(username);
    if (client) {
        client->displayAccounts();

        string accountNumber;
        cout << "Enter the account number to withdraw from: ";
        getline(cin, accountNumber);

        AccountNode* account = client->findAccount(accountNumber);
        if (account) {
            double amount;
            cout << "Enter the amount to withdraw: ";
            while (!(cin >> amount) || amount <= 0 || amount > account->balance) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\033[1;31mInvalid amount. Please enter a positive number not exceeding the balance.\033[0m\n";
                cout << "Enter the amount to withdraw: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            account->withdraw(amount);
            logAction(username, "Withdraw", "Withdrew " + to_string(amount) + " from account " + accountNumber);
            cout << "\033[1;32mWithdrawal successful.\033[0m\n";

				if(!saveToFile(fileName))
				 {
					cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
				 }

        } else {
            cout << "\033[1;31mAccount not found.\033[0m\n";
        }
    } else {
        cout << "\033[1;31mClient not found.\033[0m\n";
    }
    pauseForUser();

    FreeDeadlock();
	}
	else
	{
		cout << "File is being use by " << name << ", ssn: " << ssn << endl;
		cin.ignore(1000, '\n');
	}
}

void Bank::handleViewClientAccounts() {
    string username;
	 clearScreen();
    displayMenuHeader("View Client's Accounts");
	 cout << endl;
    cout << "Enter the client's username: ";
    getline(cin, username);
	 cout << endl;

    ClientNode* client = findClientByUsername(username);
    if (client) {
        client->displayAccounts();
    } else {
        cout << "\033[1;31mClient not found.\033[0m\n";
    }
    pauseForUser();
}

// Client-specific functions

void Bank::handleClientViewAccounts(ClientNode* client) {
    clearScreen();
	 displayMenuHeader("View Accounts");
    cout << "\033[1;34mYour Accounts:\033[0m\n\n";
    client->displayAccounts();
    pauseForUser();
}

void Bank::handleClientAddAccount(ClientNode* client, string fileName) {
	handleAddAccountToClient(client, fileName, client);
}

void Bank::handleClientDeposit(ClientNode* client, string fileName) {

	 clearScreen();
	 displayMenuHeader("Deposit To Account");
    client->displayAccounts();

    string accountNumber;
    cout << "Enter the account number to deposit into: ";
    getline(cin, accountNumber);

    AccountNode* account = client->findAccount(accountNumber);
    if (account) {
        double amount;
        cout << "Enter the amount to deposit: ";
        while (!(cin >> amount) || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[1;31mInvalid amount. Please enter a positive number.\033[0m\n";
            cout << "Enter the amount to deposit: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        account->deposit(amount);

			if(!saveToFile(fileName))
			 {
				cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
			 }

        cout << "\033[1;32mDeposit successful.\033[0m\n";
    } else {
        cout << "\033[1;31mAccount not found.\033[0m\n";
    }
    pauseForUser();
}

void Bank::handleClientWithdraw(ClientNode* client, string fileName) {

	 clearScreen();
	 displayMenuHeader("Withdraw From Account");
    client->displayAccounts();

    string accountNumber;
    cout << "Enter the account number to withdraw from: ";
    getline(cin, accountNumber);

    AccountNode* account = client->findAccount(accountNumber);
    if (account) {
		  if(account->maturityMonths <= 0)
		  {
			  if(account->accountType != "CertificateOfDeposit")
			  {
		        double amount;
		        cout << "Enter the amount to withdraw: ";
		        while (!(cin >> amount) || amount <= 0 || amount > account->balance) {
		            cin.clear();
		            cin.ignore(numeric_limits<streamsize>::max(), '\n');
		            cout << "\033[1;31mInvalid amount. Please enter a positive number not exceeding your balance.\033[0m\n";
		            cout << "Enter the amount to withdraw: ";
		        }
		        cin.ignore(numeric_limits<streamsize>::max(), '\n');

		        account->withdraw(amount);
			  }
			  else
			  {
					account->withdraw(account->balance);
			  }

				if(!saveToFile(fileName))
				 {
					cerr << "\033[1;31mFailed to save data to file.\033[0m\n";
				 }

	        cout << "\033[1;32mWithdrawal successful.\033[0m\n";
		  }
		  else
		  {
				cout << "\033[1;31mCD has not been matured. No Withdrawal.\033[0m\n";
		  }
    } else {
        cout << "\033[1;31mAccount not found.\033[0m\n";
    }
    pauseForUser();
}

int Bank::getch() {
    int ch;
    // struct to hold the terminal settings
    struct termios old_settings, new_settings;
    // take default setting in old_settings
    tcgetattr(STDIN_FILENO, &old_settings);
    // make of copy of it (Read my previous blog to know
    // more about how to copy struct)
    new_settings = old_settings;
    // change the settings for by disabling ECHO mode
    // read man page of termios.h for more settings info
    new_settings.c_lflag &= ~(ICANON | ECHO);
    // apply these new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    // now take the input in this mode
    ch = getchar();
    // reset back to default settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

    return ch;
}

string Bank::GetHiddenPassword(){

	string psswrd;
	char password[100];
	int i = 0;
	int ch;

	while ((ch = getch()) != '\n')
	{
		if (ch == 127 || ch == 8) { // handle backspace
			if (i != 0) {
				i--;
				printf("\b \b");
			}
		} else {
			password[i++] = ch;
			// echo the '*' to get feel of taking password
			printf("*");
		}
	}
	//password[i] = '\0';

	psswrd = password;

	return psswrd;
}

void Bank::SavePersonUsingData(ClientNode *head)
{
	std::ofstream ofout("deadlock.txt");

	ofout << 1 << ' ' << head->name << ' ' << head->ssn;

	ofout.close();

	return;
}

ClientNode* Bank::LoginSystem() {
	bool leave = false;
    //	bool invalid;
    do
    {
		clearScreen();
		string username, password;
		displayMenuHeader("Login System");
		cout << "\033[1;35m" << "Username: " << "\033[0m";  // Purple input prompt for Username
		getline(cin, username);
		cout << "\033[1;35m" << "Password: " << "\033[0m";  // Purple input prompt for Password
		password = GetHiddenPassword();
		 cout << endl;

		ClientNode* client = findClientByUsername(username);

		if (client && client->password == password) {
			cout << endl << endl;
		    logAction(username, "Login", "User logged in.");
			return client;
		} else {
			return nullptr;
		}
    }while(!leave);
}




void Bank::ATMFunction(){

/*
	This should concise Deposit, withdrawal Deposit function.
*/

}
