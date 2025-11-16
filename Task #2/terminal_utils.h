#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>

void gotoXY(int x, int y);
void setColor(const std::string& color);
void clearScreen();
void display(const std::string& text);

// Key reading (cross-platform)
int readKey();

// Disable / enable raw mode
void enableRawMode();
void disableRawMode();

#endif
