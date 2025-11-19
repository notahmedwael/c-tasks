#include "App.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <ncurses.h>
#include <unistd.h>
#endif

#include <vector>
#include <string>

// ---------------- Constructor / Destructor ----------------
App::App() : state(MAIN_SCREEN), selected(0) {
#ifdef _WIN32
    enableRawMode();
#else
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Selected
        init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Normal
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
    }
#endif
}

App::~App() {
#ifdef _WIN32
    disableRawMode();
#else
    endwin();
#endif
}

// ---------------- Drawing Helpers ----------------
void App::drawMainScreen(int rows, int cols) {
    std::vector<std::string> items = {"New", "Display", "Exit"};

#ifdef _WIN32
    clearScreen();
#else
    clear();
#endif

    int menu_h = items.size();
    int startY = (rows - menu_h) / 2;

    int width = 0;
    for (auto &s : items)
        if ((int)s.length() > width) width = s.length();
    width += 2;

    int startX = (cols - width) / 2;
    if (startX < 0) startX = 0;

    for (int i = 0; i < (int)items.size(); ++i) {
#ifdef _WIN32
        gotoXY(startX, startY + i);
        if (i == selected) setColor("cyan");
        else setColor("white");
        display((i == selected ? "> " : "  ") + items[i]);
#else
        if (i == selected) {
            attron(COLOR_PAIR(1));
            mvprintw(startY + i, startX, "> %s", items[i].c_str());
            attroff(COLOR_PAIR(1));
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(startY + i, startX, "  %s", items[i].c_str());
            attroff(COLOR_PAIR(2));
        }
#endif
    }

#ifdef _WIN32
    gotoXY(2, rows - 2);
    setColor("white");
    display("Use Arrow Keys to navigate, Enter to select");
#else
    attron(COLOR_PAIR(2));
    mvprintw(rows - 2, 2, "Use Arrow Keys to navigate, Enter to select");
    attroff(COLOR_PAIR(2));
    refresh();
#endif
}

void App::drawNewScreen(int rows, int cols) {
#ifdef _WIN32
    clearScreen();
    auto [columns, r] = getTerminalSize();
    gotoXY((columns - 18) / 2, rows / 2);
    setColor("cyan");
    display("Welcome to New Line");
    gotoXY(2, rows - 2);
    setColor("white");
    display("Press Backspace, Home, or Enter to return");
#else
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(rows / 2, (cols - 18) / 2, "Welcome to New Line");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
    attroff(COLOR_PAIR(2));
    refresh();
#endif
}

void App::drawDisplayScreen(int rows, int cols) {
    std::string content = "Old Screen Content";

#ifdef _WIN32
    clearScreen();
    auto [columns, r] = getTerminalSize();
    gotoXY((columns - content.length()) / 2, rows / 2);
    display(content);
    gotoXY(2, rows - 2);
    display("Press Backspace, Home, or Enter to return");
#else
    clear();
    attron(COLOR_PAIR(3));
    mvprintw(rows / 2, (cols - (int)content.length()) / 2, "%s", content.c_str());
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(2));
    mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
    attroff(COLOR_PAIR(2));
    refresh();
#endif
}

// ---------------- Input Handling ----------------
void App::handleMainInput(int ch, int rows) {
    switch (ch) {
#ifdef _WIN32
        case 1001: // UP
        case 1003: // LEFT
#else
        case KEY_UP:
        case KEY_LEFT:
#endif
            selected--;
            if (selected < 0) selected = 2;
            break;

#ifdef _WIN32
        case 1002: // DOWN
        case 1004: // RIGHT
#else
        case KEY_DOWN:
        case KEY_RIGHT:
#endif
            selected++;
            if (selected > 2) selected = 0;
            break;

#ifdef _WIN32
        case 1005: selected = 0; break;
        case 1006: selected = 2; break;
#else
        case KEY_HOME: selected = 0; break;
        case KEY_END: selected = 2; break;
#endif

        case 10:
        case 13:
            if (selected == 0) state = NEW_SCREEN;
            else if (selected == 1) state = DISPLAY_SCREEN;
            else {
#ifdef _WIN32
                clearScreen();
                auto [cols, _r] = getTerminalSize();
                gotoXY((cols - 27) / 2, rows / 2);
                display("Thanks For Using The Program");
                Sleep(800);
#else
                clear();
                mvprintw(rows / 2, (COLS - 27) / 2, "Thanks For Using The Program");
                refresh();
                usleep(800000);
                endwin();
#endif
                exit(0);
            }
            break;
    }
}

void App::handleSubScreenInput(int ch) {
    switch (ch) {
        case 8:  
        case 127:
        case 10:
        case 13:
#ifdef _WIN32
        case 1005:
#else
        case KEY_HOME:
#endif
            state = MAIN_SCREEN;
            break;
    }
}

// ---------------- Main Loop ----------------
void App::run() {
    while (true) {
#ifdef _WIN32
        auto [cols, rows] = getTerminalSize();
        clearScreen();
#else
        int rows = LINES;
        int cols = COLS;
        clear();
#endif
        if (state == MAIN_SCREEN) drawMainScreen(rows, cols);
        else if (state == NEW_SCREEN) drawNewScreen(rows, cols);
        else if (state == DISPLAY_SCREEN) drawDisplayScreen(rows, cols);

#ifdef _WIN32
        int ch = readKey();
#else
        int ch = getch();
#endif

        if (state == MAIN_SCREEN) handleMainInput(ch, rows);
        else handleSubScreenInput(ch);
    }
}
