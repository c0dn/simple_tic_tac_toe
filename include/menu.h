#ifndef MENU_H
#define MENU_H

#include <game.h>



GameResources load_game_resources();
void unload_game_resources(GameResources* resources);
void update_grid_dimensions(GameContext* context);

#endif //MENU_H
