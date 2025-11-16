#include "terminal_utils.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void gotoXY(int x, int y) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);
#else
    cout << "\033[" << y << ";" << x << "H";
#endif
}

// Converts color names to ANSI / Windows codes
void setColor(const string& color) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int code = 7; // default white

    if (color == "red") code = 12;
    else if (color == "green") code = 10;
    else if (color == "yellow") code = 14;
    else if (color == "blue") code = 9;
    else if (color == "magenta") code = 13;
    else if (color == "cyan") code = 11;
    else if (color == "white") code = 15;

    SetConsoleTextAttribute(h, code);
#else
    string code = "37"; // default: white

    if (color == "red") code = "31";
    else if (color == "green") code = "32";
    else if (color == "yellow") code = "33";
    else if (color == "blue") code = "34";
    else if (color == "magenta") code = "35";
    else if (color == "cyan") code = "36";
    else if (color == "white") code = "97";

    cout << "\033[" << code << "m";
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

void display(const string& text) {
    cout << text;
}