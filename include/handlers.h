#ifndef HANDLERS_H
#define HANDLERS_H
#include <game.h>
#include <menu.h>
#include <raylib.h>

void handle_game_click(Vector2 mouse_pos, float screen_width, float screen_height,
    const GameResources* resources, GameState* state);

void handle_menu_click(Vector2 mouse_pos, const GameResources* resources, GameState* state);
void handle_settings_menu_click(Vector2 mouse_pos, const GameResources* resources, GameState* state);
void handle_exit_menu_click(Vector2 mouse_pos, const GameResources* resources, GameState* state, bool* exit_flag);
void handle_game_mode_menu_click(Vector2 mouse_pos, const GameResources* resources, GameState* state);
void handle_instructions_menu_click(Vector2 mouse_pos, const GameResources* resources, GameState* state);
void handle_game_over_menu_click(Vector2 mouse_pos, const GameResources* resources, GameState* state);



#endif //HANDLERS_H