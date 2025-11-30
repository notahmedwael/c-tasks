// main.cpp
#include "magic_square.h"
#include <cstdio>
#include <vector>

// Cross-platform sleep
#ifdef _WIN32
    #include <windows.h>
    #include <pdcurses.h>
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

    init_ncurses();

    // For positioning on screen
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    const int cell_width = 6;
    int start_x = (max_x - n * cell_width) / 2;
    int start_y = (max_y - n - 6) / 2;
    if (start_y < 2) start_y = 2;

    draw_title(n, start_y, max_x);

    // Track occupancy without storing values (just booleans)
    std::vector<std::vector<bool>> used(n, std::vector<bool>(n, false));

    // Initial position
    int row = 0;
    int col = (n - 1) / 2;

    place_number(1, row, col, start_y, start_x, cell_width, true);
    used[row][col] = true;
    sleep_ms(400);

    // Logic for next numbers
    for (int num = 2; num <= n * n; num++) {
        int new_row = (row - 1 + n) % n;
        int new_col = (col + 1) % n;

        if (used[new_row][new_col]) {
            // Move down from original
            new_row = (row + 1) % n;
            new_col = col;
        }

        row = new_row;
        col = new_col;
        used[row][col] = true;

        place_number(num, row, col, start_y, start_x, cell_width, true);
        sleep_ms(350);
    }

    draw_complete_message(max_y, max_x);

    getch();
    cleanup_ncurses();
    return 0;
}
