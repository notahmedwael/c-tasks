#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "terminal_utils.h"

#ifndef _WIN32
#include <ncurses.h>
#include <unistd.h>
#endif

using namespace std;

enum ScreenState { MAIN, NEW_SCREEN, OLD_SCREEN };

#ifdef _WIN32
// ---------------- WINDOWS HELPERS ----------------
void drawCentered(int y, const string &text) {
    auto [cols, rows] = getTerminalSize();
    int x = (cols - text.length()) / 2;
    if (x < 0) x = 0;
    gotoXY(x, y);
    display(text);
}

void waitMs(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

#else
// ---------------- LINUX / NCURSES ----------------
void drawCentered(int y, const string &text, int color_pair = 0) {
    int cols = COLS;
    int x = (cols - text.length()) / 2;
    if (x < 0) x = 0;

    if (color_pair > 0) attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s", text.c_str());
    if (color_pair > 0) attroff(COLOR_PAIR(color_pair));
}

void waitMs(int ms) {
    usleep(ms * 1000);
}
#endif

int main() {
#ifdef _WIN32
    enableRawMode(); // optional, does nothing on Windows currently
#else
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
    }
#endif

    vector<string> options = {"New", "Display", "Exit"};
    int selected = 0;
    ScreenState state = MAIN;
    string oldScreen = "Old Screen Content";

    while (true) {
#ifdef _WIN32
        auto [cols, rows] = getTerminalSize();
        clearScreen();
#else
        int rows = LINES;
        int cols = COLS;
        clear();
#endif

        if (state == MAIN) {
            int menu_height = options.size();
            int startY = (rows - menu_height) / 2;

            int menu_width = 0;
            for (const auto &opt : options) {
                if ((int)opt.length() > menu_width) menu_width = opt.length();
            }
            menu_width += 2;
            int startX = (cols - menu_width) / 2;
            if (startX < 0) startX = 0;

            for (int i = 0; i < (int)options.size(); ++i) {
#ifdef _WIN32
                if (i == selected) setColor("cyan");
                else setColor("white");
                gotoXY(startX, startY + i);
                display((i == selected ? "> " : "  ") + options[i]);
#else
                if (i == selected) {
                    attron(COLOR_PAIR(1));
                    mvprintw(startY + i, startX, "> %s", options[i].c_str());
                    attroff(COLOR_PAIR(1));
                } else {
                    attron(COLOR_PAIR(2));
                    mvprintw(startY + i, startX, "  %s", options[i].c_str());
                    attroff(COLOR_PAIR(2));
                }
#endif
            }

#ifdef _WIN32
            gotoXY(2, rows - 2);
            display("Use Arrow Keys to navigate, Enter to select");
#else
            attron(COLOR_PAIR(2));
            mvprintw(rows - 2, 2, "Use Arrow Keys to navigate, Enter to select");
            attroff(COLOR_PAIR(2));
#endif
        }
        else if (state == NEW_SCREEN) {
            drawCentered(rows / 2, "Welcome to New Line");
#ifdef _WIN32
            gotoXY(2, rows - 2);
            display("Press Backspace, Home, or Enter to return");
#else
            attron(COLOR_PAIR(2));
            mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
            attroff(COLOR_PAIR(2));
#endif
        }
        else if (state == OLD_SCREEN) {
            drawCentered(rows / 2, oldScreen);
#ifdef _WIN32
            gotoXY(2, rows - 2);
            display("Press Backspace, Home, or Enter to return");
#else
            attron(COLOR_PAIR(2));
            mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
            attroff(COLOR_PAIR(2));
#endif
        }

#ifdef _WIN32
        int ch = readKey();
#else
        refresh();
        int ch = getch();
#endif

        // ---------- Handle input ----------
        if (state == MAIN) {
            switch (ch) {
#ifdef _WIN32
                case 1001: // UP
                case 1003: // LEFT
#else
                case KEY_UP:
                case KEY_LEFT:
#endif
                    selected--;
                    if (selected < 0) selected = options.size() - 1;
                    break;

#ifdef _WIN32
                case 1002: // DOWN
                case 1004: // RIGHT
#else
                case KEY_DOWN:
                case KEY_RIGHT:
#endif
                    selected++;
                    if (selected >= (int)options.size()) selected = 0;
                    break;

#ifdef _WIN32
                case 1005: selected = 0; break;
                case 1006: selected = options.size() - 1; break;
#else
                case KEY_HOME: selected = 0; break;
                case KEY_END: selected = options.size() - 1; break;
#endif
                case 10:  // Enter
                case 13:
                    if (selected == 0) state = NEW_SCREEN;
                    else if (selected == 1) state = OLD_SCREEN;
                    else if (selected == 2) {
#ifdef _WIN32
                        clearScreen();
                        drawCentered(rows / 2, "Thanks For Using The Program");
                        waitMs(800);
#else
                        clear();
                        drawCentered(rows / 2, "Thanks For Using The Program", 5);
                        refresh();
                        usleep(800000);
                        endwin();
#endif
                        return 0;
                    }
                    break;
            }
        } else { // NEW_SCREEN / OLD_SCREEN
            switch (ch) {
                case 8:  // Backspace
                case 127:
                case 10:
                case 13:
#ifdef _WIN32
                case 1005: // Home
#else
                case KEY_HOME:
#endif
                    state = MAIN;
                    break;
            }
        }
    }

#ifdef _WIN32
    disableRawMode();
#else
    endwin();
#endif

    return 0;
}
