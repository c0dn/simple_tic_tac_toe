#ifndef HANDLERS_H
#define HANDLERS_H
#include <buttons.h>
#include <game.h>
#include <raylib.h>

void handle_game_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);
void handle_clicks(Vector2 click_pos, const GameResources* resources, GameContext* context, const Button* buttons, size_t count);
void handle_menu_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);
void handle_music_toggle(const GameResources* resources, GameContext* context);

#endif //HANDLERS_H