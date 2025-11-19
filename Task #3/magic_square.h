#ifndef MAGIC_SQUARE_H
#define MAGIC_SQUARE_H

void init_ncurses();
void cleanup_ncurses();

// New helpers:
void draw_title(int n, int start_y, int max_x);
void place_number(int num, int row, int col,
                  int start_y, int start_x,
                  int cell_width, bool highlight);
void draw_complete_message(int max_y, int max_x);

#endif
