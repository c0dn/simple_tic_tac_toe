#ifndef RENDER_H
#define RENDER_H

#define START_Y 2
#define START_X 5
#define CELL_WIDTH 6
#define CELL_HEIGHT 3
#define GRID_WIDTH (CELL_WIDTH * 3)
#define GRID_HEIGHT (CELL_HEIGHT * 3)
#include <game.h>
#include <menu.h>

void render_grid(float screen_height, float screen_width);
void render_menu(float screen_height, float screen_width,
                 GameResources* resources, GameState* state);
void render_game_over(float screen_height, float screen_width,
                      GameResources* resources, GameState* state);
void render_instructions(float screen_height, float screen_width,
                         GameResources* resources, GameState* game_state);
void render_settings(float screen_height, float screen_width,
                     GameResources* resources, GameState* state);
void render_exit(float screen_height, float screen_width,
                 GameResources* resources, GameState* state);
void render_game_mode_choice(float screen_height, float screen_width,
                             GameResources* resources, GameState* state);
#endif //RENDER_H
