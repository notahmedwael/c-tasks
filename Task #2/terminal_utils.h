#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>

void gotoXY(int x, int y);
void setColor(const std::string& color);
void clearScreen();
void display(const std::string& text);

#endif
