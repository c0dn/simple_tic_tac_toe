#ifndef BUTTONS_H
#define BUTTONS_H
#include "common.h"

typedef struct
{
    Rectangle* rect;
    const char* text;
    Color color;
    const float width;
    const float height;
    const float first_render_offset;
    const Color clickColor;
    const bool override_default_colors;
    const ComponentPadding padding;
    const bool rounded;
    const int font_size;
    void (*action)(const GameResources*, GameContext*);
} Button;

extern Button GAME_MODE_BUTTONS[3];
extern Button EXIT_CONFIRMATION_BUTTONS[2];
extern Button INSTRUCTIONS_BUTTONS[1];
extern Button MAIN_MENU_BUTTONS[4];
extern Button GAME_OVER_BUTTONS[2];
extern Button IN_GAME_BUTTONS[1];

#endif // BUTTONS_H
