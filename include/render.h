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


typedef struct {
    Rectangle rect;
    const char* text;
    Color color;
    void (*action)(const GameResources*, GameState*);
} Button;

void render_grid(const GameResources* resources);
void render_menu(
    const GameResources* resources);
void render_game_over(
    const GameState* state);
void render_instructions(
    const GameResources* resources);
void render_settings(void);
void render_exit(void);
void render_game_mode_choice(void);
#endif //RENDER_H
