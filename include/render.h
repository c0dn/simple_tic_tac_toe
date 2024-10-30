#ifndef RENDER_H
#define RENDER_H

#define START_Y 2
#define START_X 5
#define CELL_WIDTH 6
#define CELL_HEIGHT 3
#define GRID_WIDTH (CELL_WIDTH * 3)
#define GRID_HEIGHT (CELL_HEIGHT * 3)
#include <game.h>

void render_grid(int screen_height, int screen_width);
void render_menu(int screen_height, int screen_width);
void render_game_over(int screen_height, int screen_width, GameState state);


#endif //RENDER_H
