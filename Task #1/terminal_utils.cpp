#include "terminal_utils.h"
#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

#ifdef _WIN32
static unordered_map<string, int> winColors = {
    {"black", 0}, {"blue", 1}, {"green", 2}, {"cyan", 3},
    {"red", 4}, {"magenta", 5}, {"yellow", 6}, {"white", 7},
    {"bright_black", 8}, {"bright_blue", 9}, {"bright_green", 10},
    {"bright_cyan", 11}, {"bright_red", 12}, {"bright_magenta", 13},
    {"bright_yellow", 14}, {"bright_white", 15}
};

void printAt(int x, int y, const string& text, const string& colorName) {
    clearScreen();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (winColors.count(colorName))
        SetConsoleTextAttribute(hConsole, winColors[colorName]);
    else
        SetConsoleTextAttribute(hConsole, 7);

    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, pos);
    cout << text;
    clearScreen();
}

void clearScreen() {
    system("cls");
}

#else
// Linux / macOS
static unordered_map<string, int> colors = {
    {"black", 30}, {"red", 31}, {"green", 32}, {"yellow", 33},
    {"blue", 34}, {"magenta", 35}, {"cyan", 36}, {"white", 37},
    {"bright_black", 90}, {"bright_red", 91}, {"bright_green", 92}, {"bright_yellow", 93},
    {"bright_blue", 94}, {"bright_magenta", 95}, {"bright_cyan", 96}, {"bright_white", 97}
};

void printAt(int x, int y, const string& text, const string& colorName) {
    int code = 37; // default white
    if (colors.count(colorName))
        code = colors[colorName];

    cout << "\033[" << y << ";" << x << "H";
    cout << "\033[" << code << "m" << text << "\033[0m";
    cout.flush();
}

void clearScreen() {
    cout << "\033[2J\033[H";
}
#endif