// magic_square.h
#ifndef MAGIC_SQUARE_H
#define MAGIC_SQUARE_H

#include <vector>
#include <string>

void init_ncurses();
void draw_grid(const std::vector<std::vector<int>>& grid, int n, int current_num);
void cleanup_ncurses();

#endif // MAGIC_SQUARE_H