#ifndef RENDER_H
#define RENDER_H
#include <notcurses/notcurses.h>

#define START_Y 2
#define START_X 5
#define CELL_WIDTH 6
#define CELL_HEIGHT 3
#define GRID_WIDTH (CELL_WIDTH * 3)
#define GRID_HEIGHT (CELL_HEIGHT * 3)


void render_grid(struct ncplane* n);

void draw_symbol_in_cell(struct ncplane* n, int cell_row, int cell_column, wchar_t symbol, FILE* app_log);

void display_message(struct ncplane* n, const wchar_t* message);



#endif //RENDER_H
