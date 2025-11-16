#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>
#include <utility>

// Basic screen helpers
void gotoXY(int x, int y);
void setColor(const std::string& color);
void clearScreen();
void display(const std::string& text);

// Terminal size
std::pair<int,int> getTerminalSize();

// Input helpers
int readKey(); // returns ASCII or special codes (1001..1006 for arrows/home/end)

// Raw mode control (Linux)
void enableRawMode();
void disableRawMode();

#endif // TERMINAL_UTILS_H