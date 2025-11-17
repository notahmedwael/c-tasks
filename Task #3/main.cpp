// main.cpp
#include "magic_square.h"
#include <vector>
#include <cstdio>

// Cross-platform sleep
#ifdef _WIN32
    #include <windows.h>
    #include <curses.h>
    #define sleep_ms(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <ncurses.h>
    #define sleep_ms(ms) usleep((ms) * 1000)
#endif

int main() {
    int n;
    printf("Enter odd n (3, 5, 7, 9...): ");
    if (scanf("%d", &n) != 1 || n % 2 == 0 || n < 1) {
        printf("Error: Please enter a positive odd integer!\n");
        return 1;
    }

    std::vector<std::vector<int>> grid(n, std::vector<int>(n, 0));

    init_ncurses();

    int row = 0;
    int col = (n - 1) / 2;
    grid[row][col] = 1;
    draw_grid(grid, n, 1);
    sleep_ms(500);

    for (int num = 2; num <= n * n; ++num) {
        int new_row = (row - 1 + n) % n;
        int new_col = (col + 1) % n;

        if (grid[new_row][new_col] != 0) {
            new_row = (row + 1) % n;
            new_col = col;
        }

        grid[new_row][new_col] = num;
        row = new_row;
        col = new_col;

        draw_grid(grid, n, num);
        sleep_ms(400);
    }

    draw_grid(grid, n, n * n);

    attron(COLOR_PAIR(5) | A_BOLD | A_BLINK);
    std::string done = "MAGIC SQUARE COMPLETE!";
    mvprintw(LINES - 4, (COLS - (int)done.length()) / 2, "%s", done.c_str());
    attroff(COLOR_PAIR(5) | A_BOLD | A_BLINK);

    mvprintw(LINES - 2, 2, "Press any key to exit...");
    refresh();
    getch();

    cleanup_ncurses();
    return 0;
}