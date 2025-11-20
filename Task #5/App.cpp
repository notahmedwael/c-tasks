#include "App.h"
#include <cctype>
#include <cstdlib>

#ifdef _WIN32
#include <pdcurses.h>
#include <windows.h>
#else
#include <ncurses.h>
#include <unistd.h>
#endif

// ---------------- Constructor / Destructor ----------------
App::App() : state(MAIN_SCREEN), selected(0) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Selected
        init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Normal
        init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Display heading
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);    // Error
    }
}

App::~App() {
    endwin();
}

// ---------------- Validators ----------------
bool App::isInteger(const std::string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!std::isdigit((unsigned char)c)) return false;
    return true;
}

bool App::isNumber(const std::string &s) {
    if (s.empty()) return false;
    bool dot = false;
    for (char c : s) {
        if (c == '.') {
            if (dot) return false;
            dot = true;
        } else if (!std::isdigit((unsigned char)c)) return false;
    }
    return true;
}

// ---------------- Input Helper ----------------
std::string App::inputString(const std::string &prompt, int rows, int cols) {
    const int MAXLEN = 256;
    char buf[MAXLEN];

    clear();
    mvprintw(rows / 2 - 1, (cols - (int)prompt.length()) / 2, "%s", prompt.c_str());
    mvprintw(rows / 2 + 1, (cols - 40) / 2, "(Leave blank + Enter to cancel)");
    move(rows / 2, (cols - 30) / 2);
    clrtoeol();
    echo();
    nocbreak();
    refresh();
    getnstr(buf, MAXLEN - 1);
    noecho();
    cbreak();

    return std::string(buf);
}

// ---------------- Drawing Helpers ----------------
void App::drawMainScreen(int rows, int cols) {
    std::vector<std::string> items = {"New", "Display", "Exit"};
    clear();

    int menu_h = items.size();
    int startY = (rows - menu_h) / 2;
    int width = 0;
    for (auto &s : items) if ((int)s.length() > width) width = s.length();
    width += 2;
    int startX = (cols - width) / 2;
    if (startX < 0) startX = 0;

    for (int i = 0; i < (int)items.size(); ++i) {
        if (i == selected) {
            attron(COLOR_PAIR(1));
            mvprintw(startY + i, startX, "> %s", items[i].c_str());
            attroff(COLOR_PAIR(1));
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(startY + i, startX, "  %s", items[i].c_str());
            attroff(COLOR_PAIR(2));
        }
    }

    attron(COLOR_PAIR(2));
    mvprintw(rows - 2, 2, "Use Arrow Keys to navigate, Enter to select");
    attroff(COLOR_PAIR(2));
    refresh();
}

void App::drawNewScreen(int rows, int cols) {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(rows / 2 - 2, (cols - 14) / 2, "Add New Employee");
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(2));
    mvprintw(rows - 2, 2, "Leave field blank + Enter to cancel at any time");
    attroff(COLOR_PAIR(2));
    refresh();
}

void App::drawDisplayScreen(int rows, int cols) {
    clear();
    std::string hdr = "Employees (" + std::to_string(employees.size()) + ")";
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(1, (cols - (int)hdr.length()) / 2, "%s", hdr.c_str());
    attroff(COLOR_PAIR(3) | A_BOLD);

    int line = 3;
    if (employees.empty()) {
        attron(COLOR_PAIR(2));
        mvprintw(rows / 2, (cols - 17) / 2, "No employees found!");
        attroff(COLOR_PAIR(2));
    } else {
        for (size_t i = 0; i < employees.size(); ++i) {
            const Employee &e = employees[i];
            char buf[200];
            snprintf(buf, sizeof(buf), "%zu) %s %s | %c | Age:%d | Salary: %.2f",
                     i + 1, e.firstName.c_str(), e.lastName.c_str(), e.gender, e.age, e.salary);
            mvprintw(line++, 2, "%s", buf);
            if (line >= rows - 2) break;
        }
    }

    attron(COLOR_PAIR(2));
    mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
    attroff(COLOR_PAIR(2));
    refresh();
}

// ---------------- Input Handling ----------------
void App::handleMainInput(int ch, int rows) {
    switch (ch) {
        case KEY_UP: case KEY_LEFT: selected--; if (selected < 0) selected = 2; break;
        case KEY_DOWN: case KEY_RIGHT: selected++; if (selected > 2) selected = 0; break;
        case KEY_HOME: selected = 0; break;
        case KEY_END: selected = 2; break;
        case 10: case 13: // Enter
            if (selected == 0) state = NEW_SCREEN;
            else if (selected == 1) state = DISPLAY_SCREEN;
            else {
                clear();
                mvprintw(rows / 2, (COLS - 27) / 2, "Thanks For Using The Program");
                refresh();
                #ifdef _WIN32
                Sleep(800);
                #else
                usleep(800000);
                #endif
                endwin();
                exit(0);
            }
            break;
    }
}

void App::handleSubScreenInput(int ch) {
    switch (ch) {
        case 8: case 127: case 10: case 13: case KEY_HOME:
            state = MAIN_SCREEN;
            break;
    }
}

// ---------------- Main Loop ----------------
void App::run() {
    while (true) {
        int rows = LINES;
        int cols = COLS;

        if (state == MAIN_SCREEN) drawMainScreen(rows, cols);

        else if (state == NEW_SCREEN) {
            drawNewScreen(rows, cols);
            Employee e;

            std::string s = inputString("First Name: ", rows, cols);
            if (s.empty()) { state = MAIN_SCREEN; continue; }
            e.firstName = s;

            s = inputString("Last Name: ", rows, cols);
            if (s.empty()) { state = MAIN_SCREEN; continue; }
            e.lastName = s;

            while (true) {
                s = inputString("Gender (M/F): ", rows, cols);
                if (s.empty()) { state = MAIN_SCREEN; break; }
                char g = s[0];
                if (g == 'M' || g == 'm' || g == 'F' || g == 'f') {
                    e.gender = std::toupper((unsigned char)g);
                    break;
                }
                attron(COLOR_PAIR(5));
                mvprintw(rows - 3, 2, "Invalid gender, enter M or F. Press Enter to retry.");
                attroff(COLOR_PAIR(5));
                refresh();
            }
            if (state == MAIN_SCREEN) continue;

            while (true) {
                s = inputString("Age: ", rows, cols);
                if (s.empty()) { state = MAIN_SCREEN; break; }
                if (isInteger(s)) {
                    int age = atoi(s.c_str());
                    if (age > 0 && age < 120) { e.age = age; break; }
                }
                attron(COLOR_PAIR(5));
                mvprintw(rows - 3, 2, "Invalid age (positive integer). Press Enter to retry.");
                attroff(COLOR_PAIR(5));
                refresh();
            }
            if (state == MAIN_SCREEN) continue;

            while (true) {
                s = inputString("Salary: ", rows, cols);
                if (s.empty()) { state = MAIN_SCREEN; break; }
                if (isNumber(s)) {
                    double sal = atof(s.c_str());
                    if (sal >= 0.0) { e.salary = sal; break; }
                }
                attron(COLOR_PAIR(5));
                mvprintw(rows - 3, 2, "Invalid salary (number). Press Enter to retry.");
                attroff(COLOR_PAIR(5));
                refresh();
            }
            if (state == MAIN_SCREEN) continue;

            employees.push_back(e);
            mvprintw(rows - 3, 2, "Employee added! Press 'y' to add another, any other key to return.");
            refresh();
            int k = getch();
            state = (k == 'y' || k == 'Y') ? NEW_SCREEN : MAIN_SCREEN;
        }

        else if (state == DISPLAY_SCREEN) drawDisplayScreen(rows, cols);

        int ch = getch();
        if (state == MAIN_SCREEN) handleMainInput(ch, rows);
        else handleSubScreenInput(ch);
    }
}
