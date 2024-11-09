#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>

// Terminal width for centering
const int TERMINAL_WIDTH = 73;  // Adjusted for border characters

// Function to center text based on terminal width
std::string centerText(const std::string& text, int width = TERMINAL_WIDTH);

// Clear screen function for cross-platform
void clearScreen();

// Input validation functions
bool isNumber(const std::string& str);

int getValidatedChoice(int minChoice, int maxChoice);

// Display menu header
void displayMenuHeader(const std::string& title);

// Pause for user input
void pauseForUser();

bool ReadDeadlockBool(std::string &name, int &ssn);

void FreeDeadlock();

std::string DataEncryption(std::string data);

std::string DataDecryption(std::string data);

/*
 * This function prints a variable number of arguments to a specified file,
 * ensuring that each argument is separated by a space and no trailing space
 * is added at the end. The function uses variadic templates and a fold
 * expression for efficient argument handling.
 *
 * Parameters:
 * - fileName: The name of the file to write to.
 * - args: A variable number of arguments to be printed to the file.
 */
template <typename... Types>
void printToTxt(const std::string& fileName, const Types&... args) {
    const char* separator = "";  // Separator between arguments (no space before the first argument)
    std::ofstream out(fileName, std::ios::app); // Append mode for continuous logs

    // Check if the file was successfully opened
    if (!out) {
        std::cerr << "Error opening file: " << fileName << std::endl; // Output an error message if file opening fails
        return; // Exit the function early if the file can't be opened
    }

    /*
     * Fold expression to output each argument:
     * - `out << separator << args`: Prints each argument, preceded by the current separator
     * - `separator = " "`: After the first argument, set the separator to a space for subsequent arguments
     */
    ((out << separator << args, separator = " "), ...); // Efficiently print arguments with spaces in between

    out << std::endl;  // Add a newline character at the end of the file
    out.close(); // Close the file stream after writing
}

// Function to log actions, using printToTxt to handle formatted logging.
void logAction(const std::string& username, const std::string& action, const std::string& details);

#endif // UTILS_H
