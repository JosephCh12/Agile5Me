#include "Utils.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <fstream>
#include <string>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // ANSI escape codes to clear screen and move cursor to top-left corner
    std::cout << "\033[2J\033[1;1H";
#endif
}

std::string centerText(const std::string& text, int width) {
    int len = text.length();
    if (len >= width) {
        return text;
    } else {
        int padding = (width - len) / 2;
        return std::string(padding, ' ') + text + std::string(padding + (width - len) % 2, ' ');
    }
}

bool isNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

int getValidatedChoice(int minChoice, int maxChoice) {
    int choice;
    std::string input;
    do {
        std::cout << "\033[1;35mEnter your choice: \033[0m"; // Purple for input prompt
        std::getline(std::cin, input);
        if (!isNumber(input) || input.empty()) {
            choice = 0;
        } else {
            choice = std::stoi(input);
        }
        if (choice < minChoice || choice > maxChoice) {
            std::cout << "\033[1;31mInvalid choice. Please enter a number between "
                      << minChoice << " and " << maxChoice << ".\033[0m\n";
        }
    } while (choice < minChoice || choice > maxChoice);
    return choice;
}

void displayMenuHeader(const std::string& title) {
    const std::string borderTop    = "╔═════════════════════════════════════════════════════════════════════════╗\n";
    const std::string borderBottom = "╚═════════════════════════════════════════════════════════════════════════╝\n";
    std::cout << "\033[1;34m";  // Bold Blue text for border
    std::cout << borderTop;
    std::cout << "║" << "\033[1;32m" << centerText(title, TERMINAL_WIDTH) << "\033[1;34m║\n"; // Green text for header, blue outside
    std::cout << borderBottom;
    std::cout << "\033[0m";  // Reset text
}

void pauseForUser() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

bool ReadDeadlockBool(std::string &name, int &ssn)
{
	bool open;

	std::ifstream ifin("deadlock.txt");

	ifin >> open >> name >> ssn;

	ifin.close();

	return open;
}

void FreeDeadlock()
{
	std::ofstream ofout("deadlock.txt");

	ofout << 0 << ' ' << '*' << ' ' << '*';

	ofout.close();

	return;
}

std::string DataEncryption(std::string data)
{
	std::string alphabet {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890"};
	std::string key      {"XZNLWEBGJHQDYVTKFUOMPCIASRrsaicpmoufktvydqhjgbewlnzx:$#%&!*~_^+"};
	std::string encryptedMessage;

	for(char c:data){
		size_t position = alphabet.find(c);
		if (position != std::string::npos){
			char newChar {key.at(position)};
			encryptedMessage += newChar;
		} else{
			encryptedMessage += c;
		}
	}

	return encryptedMessage;
}

std::string DataDecryption(std::string data)
{
	std::string alphabet {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890"};
	std::string key      {"XZNLWEBGJHQDYVTKFUOMPCIASRrsaicpmoufktvydqhjgbewlnzx:$#%&!*~_^+"};
	std::string decryptedMessage;

	for(char c:data){
		size_t position = key.find(c);
		if (position != std::string::npos){
			char newChar {alphabet.at(position)};
			decryptedMessage += newChar;
		} else{
			decryptedMessage += c;
		}
	}

	return decryptedMessage;
}

/*
 * Logs user actions with a timestamp, username, action type, and additional details.
 * This function centralizes logging to a specific log file, "user_activity_log.txt".
 *
 * Parameters:
 * - username: Name of the user performing the action
 * - action: Description of the action (e.g., "Login", "Deposit")
 * - details: Additional details about the action
 */
void logAction(const std::string& username, const std::string& action, const std::string& details) {
	time_t now = time(nullptr);
	printToTxt("user_activity_log.txt", ctime(&now), "- User:", username, "- Action:", action, "- Details:", details);
}
