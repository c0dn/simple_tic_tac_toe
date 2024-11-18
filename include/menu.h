#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include "neural.h"


typedef struct {
    Music background_music;
    Sound fx_click;
    Sound fx_symbol;
    Sound fx_win;
    Sound fx_draw;
    Texture2D main_menu_img;
    Texture2D instructions_1;
    Texture2D music_on;
    Texture2D music_off;
    Texture2D instructions_2;
    AiModels *models;
} GameResources;

typedef struct {
    Color background_color;
    Color primary_btn_color;
    Color btn_clicked_color;
} UiOptions;

GameResources load_game_resources();
void unload_game_resources(GameResources* resources);


#endif //MENU_H
