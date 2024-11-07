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


typedef struct
{
    const float left;
    const float right;
    const float up;
    const float down;
} ComponentPadding;

// For vertical padding (up and down same)
#define PaddingY(value) (ComponentPadding){ 0, 0, value, value }

// For horizontal padding (left and right same)
#define PaddingX(value) (ComponentPadding){ value, value, 0, 0 }

// For uniform padding in all directions
#define Padding(value) (ComponentPadding){ value, value, value, value }


typedef struct {
    Rectangle rect;
    const char* text;
    Color color;
    const float width;
    const float height;
    const float first_render_offset;
    const Color clickColor;
    const ComponentPadding padding;
    const bool rounded;
    const int font_size;
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
