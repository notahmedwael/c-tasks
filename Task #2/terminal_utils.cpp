#include "terminal_utils.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

using namespace std;

#ifdef _WIN32
DWORD originalMode;
#else
termios originalTermios;
#endif

void enableRawMode() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(h, &originalMode);
    DWORD mode = originalMode;
    mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(h, mode);
#else
    tcgetattr(STDIN_FILENO, &originalTermios);
    termios raw = originalTermios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
}

void disableRawMode() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(h, originalMode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
#endif
}

int readKey() {
#ifdef _WIN32
    INPUT_RECORD record;
    DWORD count;
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    while (true) {
        ReadConsoleInput(h, &record, 1, &count);
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
            int key = record.Event.KeyEvent.wVirtualKeyCode;

            switch(key) {
                case VK_UP: return 1001;
                case VK_DOWN: return 1002;
                case VK_LEFT: return 1003;
                case VK_RIGHT: return 1004;
                case VK_HOME: return 1005;
                case VK_END: return 1006;
                case VK_RETURN: return 13;
                case VK_BACK: return 8;
            }
        }
    }
#else
    char c;
    read(STDIN_FILENO, &c, 1);
    if (c == 27) { // escape
        char seq[2];
        read(STDIN_FILENO, &seq[0], 1);
        read(STDIN_FILENO, &seq[1], 1);

        if (seq[0] == '[') {
            if (seq[1] == 'A') return 1001; // Up
            if (seq[1] == 'B') return 1002; // Down
            if (seq[1] == 'C') return 1004; // Right
            if (seq[1] == 'D') return 1003; // Left
            if (seq[1] == 'H') return 1005; // Home
            if (seq[1] == 'F') return 1006; // End
        }
    }
    return c;
#endif
}

void gotoXY(int x, int y) {
#ifdef _WIN32
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
    cout << "\033[" << y << ";" << x << "H";
#endif
}

void setColor(const string& color) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int code = 7;

    if (color == "red") code = 12;
    else if (color == "green") code = 10;
    else if (color == "yellow") code = 14;
    else if (color == "blue") code = 9;
    else if (color == "cyan") code = 11;
    else if (color == "magenta") code = 13;
    else if (color == "white") code = 15;

    SetConsoleTextAttribute(h, code);
#else
    string code = "37";

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
    cout << "\033[2J\033[H";
#endif
}

void display(const string& text) {
    cout << text;
}
