#include "terminal_utils.h"
#include <iostream>
#include <string>
#include <utility>

#ifdef _WIN32
// ------------------ WINDOWS VERSION ------------------
#include <windows.h>

void clearScreen() {
    system("cls");
}

void gotoXY(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(const std::string &color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (color == "red") SetConsoleTextAttribute(h, 12);
    else if (color == "green") SetConsoleTextAttribute(h, 10);
    else if (color == "yellow") SetConsoleTextAttribute(h, 14);
    else if (color == "cyan") SetConsoleTextAttribute(h, 11);
    else SetConsoleTextAttribute(h, 15);
}

void display(const std::string &text) {
    std::cout << text;
}

std::pair<int,int> getTerminalSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {cols, rows};
}

int readKey() {
    INPUT_RECORD record;
    DWORD count;

    while (true) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &count);
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
            auto key = record.Event.KeyEvent.wVirtualKeyCode;

            if (key == VK_UP) return 1001;
            if (key == VK_DOWN) return 1002;
            if (key == VK_LEFT) return 1003;
            if (key == VK_RIGHT) return 1004;
            if (key == VK_HOME) return 1005;
            if (key == VK_END) return 1006;

            return record.Event.KeyEvent.uChar.AsciiChar;
        }
    }
}

void enableRawMode() {}
void disableRawMode() {}

#else
// ------- LINUX VERSION -------
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <iostream>

static struct termios originalTermios;
static bool rawModeEnabled = false;

// ------------ Raw Mode ------------
void enableRawMode() {
    if (rawModeEnabled) return;

    struct termios raw;
    tcgetattr(STDIN_FILENO, &originalTermios);
    raw = originalTermios;

    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;   
    raw.c_cc[VTIME] = 1;  

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    rawModeEnabled = true;
}

void disableRawMode() {
    if (rawModeEnabled)
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
    rawModeEnabled = false;
}

// ------------ Terminal Size ------------
std::pair<int,int> getTerminalSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return { w.ws_col, w.ws_row };
}

// ------------ Screen Functions ------------
void clearScreen() {
    write(STDOUT_FILENO, "\033[2J\033[H", 7);
}

void gotoXY(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void setColor(const std::string &color) {
    if (color == "red")      printf("\033[31m");
    else if (color == "green") printf("\033[32m");
    else if (color == "yellow") printf("\033[33m");
    else if (color == "cyan") printf("\033[36m");
    else printf("\033[37m"); // white
}

void display(const std::string &text) {
    std::cout << text;
}

// ------------ readKey() (FIXED) ------------
int readKey() {
    // Raw mode is already enabled in main(), don't call it again here
    
    unsigned char c;
    int n = read(STDIN_FILENO, &c, 1);

    if (n <= 0) return -1;

    // ESC sequence
    if (c == 27) {
        unsigned char seq[2];

        if (read(STDIN_FILENO, &seq[0], 1) <= 0) return 27;
        if (read(STDIN_FILENO, &seq[1], 1) <= 0) return 27;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 1001; // UP
                case 'B': return 1002; // DOWN
                case 'D': return 1003; // LEFT
                case 'C': return 1004; // RIGHT
                case 'H': return 1005; // HOME
                case 'F': return 1006; // END
            }
        }
        return 27;
    }

    return c; // regular ASCII key (ENTER = 13)
}

#endif