#include "magic_square.h"

#ifdef _WIN32
    #include <pdcurses.h>
#else
    #include <ncurses.h>
#endif

#include <string>

void init_ncurses() {
    initscr();
    noecho();
    curs_set(0);
    cbreak();

    start_color();
    use_default_colors();

    init_pair(1, COLOR_CYAN,    -1); // Normal numbers
    init_pair(2, COLOR_YELLOW,  -1); // Current number highlight
    init_pair(3, COLOR_GREEN,   -1); // Title
    init_pair(5, COLOR_MAGENTA, -1); // Complete
}

void cleanup_ncurses() {
    endwin();
}

void draw_title(int n, int start_y, int max_x) {
    clear();
    std::string t = "Magic Square " + std::to_string(n) + " x " + std::to_string(n);
    int mc = n * (n * n + 1) / 2;
    std::string m = "Magic Constant = " + std::to_string(mc);

    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(start_y - 3, (max_x - t.length()) / 2, "%s", t.c_str());
    mvprintw(start_y - 1, (max_x - m.length()) / 2, "%s", m.c_str());
    attroff(COLOR_PAIR(3) | A_BOLD);

    refresh();
}

void place_number(int num, int row, int col, int start_y, int start_x, int cw, bool highlight) {
    int screen_y = start_y + row;
    int screen_x = start_x + col * cw;

    if (highlight) attron(COLOR_PAIR(2) | A_BOLD | A_REVERSE);
    else           attron(COLOR_PAIR(1) | A_BOLD);

    mvprintw(screen_y, screen_x, " %3d ", num);

    if (highlight) attroff(COLOR_PAIR(2) | A_BOLD | A_REVERSE);
    else           attroff(COLOR_PAIR(1) | A_BOLD);

    refresh();
}

void draw_complete_message(int max_y, int max_x) {
    attron(COLOR_PAIR(5) | A_BOLD | A_BLINK);
    std::string done = "MAGIC SQUARE COMPLETE!";
    mvprintw(max_y - 4, (max_x - done.length()) / 2, "%s", done.c_str());
    attroff(COLOR_PAIR(5) | A_BOLD | A_BLINK);

    mvprintw(max_y - 2, 2, "Press any key to exit...");
    refresh();
}
