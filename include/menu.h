#ifndef MENU_H
#define MENU_H

#include <raylib.h>


typedef struct {
    Music background_music;
    Sound fx_click;
    Sound fx_symbol;
    Sound fx_win;
    Sound fx_draw;
    Texture2D main_menu_img;
    Texture2D instructions_1;
    Texture2D instructions_2;
} GameResources;

typedef struct {
    Color background_color;
} UiOptions;

GameResources load_game_resources(int screen_width, int screen_height);
void unload_game_resources(const GameResources* resources);


#endif //MENU_H
