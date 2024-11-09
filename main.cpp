#include "Bank.h"
#include "Utils.h"
#include <iostream>
#include <ctime>


/*
FOR RUN 4:

	TODO:
			Clean up the menu system.
			Add Colors and design.
			Add transfer funds function.
			Add the maturity months implementation. (No one should be able to make withdrawal before maturity months, also there is a minimum balance)
			Add ATM function for employee/manager.
			Add Failed/Successful Login attempts in log file.
			Display Logs.
			New Features: --------------------------------

	ASSIGNES:
			Vedant & Joseph: ATM function, Cleaning up the UI and adding colors and design, Add the Login attempt logs and display it.
			Walter & Landon: Maturity Months Implementation, Transfer Funds.

*/





int main() {
    Bank bank;
    std::string filename = "accounts.txt";  // File where data is stored
    ClientNode* current;

    // Load data from accounts.txt
    if (!bank.loadFromFile(filename)) {
        std::cerr << "\033[1;31mFailed to load data from file.\033[0m\n";
    }

    // Initialize the account number generator
    bank.initializeAccountNumberGenerator();

    bool exitProgram = false;
    bool invalid;

    while (!exitProgram) {
        current = bank.LoginSystem();

        if(current)
        {
            switch (current->role) {
                case 0:
                {
                    bank.clientInterface(current, filename);
                }
                break;
                default:
                    bank.employeeInterface(current, filename);
            }
        }
        else
        {
          	  std::cout << "\033[1;31mInvalid username or password.\033[0m\n";
            std::cout << "Would you like to try again? (y/n): ";
            char choice;
            do
            {
                invalid = true;
                std::cin >> choice;
                std::cin.ignore(1000, '\n');

                choice = tolower(choice);
                switch(choice)
                {
                    case 'y':
                        invalid = false;
                    break;
                    case 'n':
                        exitProgram = true;
                    invalid = false;
                    break;
                    default:
                        std::cout << "\033[1;31mInvalid choice. Please try again: \033[0m";
                }
            }while(invalid);
        }

        // Add 4 empty lines between menu functions
        std::cout << "\n\n\n\n";
    }

    return 0;
}
