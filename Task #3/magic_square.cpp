// magic_square.cpp
#include "magic_square.h"

// Use curses.h for both platforms (ncurses on Linux, pdcurses on Windows)
#ifdef _WIN32
    #include <curses.h>
#else
    #include <ncurses.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <string>

void init_ncurses() {
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    start_color();
    use_default_colors();

    init_pair(1, COLOR_CYAN,    -1);   // Normal numbers
    init_pair(2, COLOR_YELLOW,  -1);   // Current number
    init_pair(3, COLOR_GREEN,   -1);   // Title
    init_pair(4, COLOR_WHITE,   -1);   // Empty cells
    init_pair(5, COLOR_MAGENTA, -1);   // Complete message
}

void cleanup_ncurses() {
    endwin();
}

void draw_grid(const std::vector<std::vector<int>>& grid, int n, int current_num) {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    const int cell_width = 6;
    const int grid_width = n * cell_width;
    int start_x = (max_x - grid_width) / 2;
    int start_y = (max_y - n - 6) / 2;
    if (start_y < 2) start_y = 2;

    // Title
    std::string title = "Magic Square " + std::to_string(n) + "x" + std::to_string(n);
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(start_y - 3, (max_x - (int)title.length()) / 2, "%s", title.c_str());
    attroff(COLOR_PAIR(3) | A_BOLD);

    // Magic constant
    int magic_const = n * (n * n + 1) / 2;
    std::string magic_str = "Magic Constant = " + std::to_string(magic_const);
    attron(COLOR_PAIR(3));
    mvprintw(start_y - 1, (max_x - (int)magic_str.length()) / 2, "%s", magic_str.c_str());
    attroff(COLOR_PAIR(3));

    // Draw grid
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int x = start_x + j * cell_width;
            int y = start_y + i;

            if (grid[i][j] == 0) {
                attron(COLOR_PAIR(4) | A_DIM);
                mvprintw(y, x, "  ·  ");
                attroff(COLOR_PAIR(4) | A_DIM);
            } else {
                if (grid[i][j] == current_num) {
                    attron(COLOR_PAIR(2) | A_BOLD | A_REVERSE);
                    mvprintw(y, x, " %3d ", grid[i][j]);
                    attroff(COLOR_PAIR(2) | A_BOLD | A_REVERSE);
                } else {
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(y, x, "%4d ", grid[i][j]);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                }
            }
        }
    }

    // Status line
    std::string status = (current_num == n * n) ? "Complete!" : "Placing number: " + std::to_string(current_num);
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(start_y + n + 2, (max_x - (int)status.length()) / 2, "%s", status.c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);

    refresh();
}