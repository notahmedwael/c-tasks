#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>

// Print text at (x,y) with color by name
void printAt(int x, int y, const std::string& text, const std::string& colorName = "white");

// Clear the terminal screen
void clearScreen();

#endif // TERMINAL_UTILS_H