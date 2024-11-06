#ifndef HANDLERS_H
#define HANDLERS_H
#include <game.h>
#include <menu.h>
#include <raylib.h>

void handle_mouse_click(Vector2 mouse_pos, float screen_width, float screen_height,
    const GameResources* resources, GameState* state);

static void start_easy_mode(const GameResources* res, GameState* game_state);
static void start_medium_mode(const GameResources* res, GameState* game_state);
static void start_hard_mode(const GameResources* res, GameState* game_state);

// Window and menu navigation handlers
static void close_window(const GameResources* res, GameState* _game_state);
static void return_to_menu(const GameResources* res, GameState* game_state);

static void start_1player(const GameResources* res, GameState* game_state);
static void start_2player(const GameResources* res, GameState* game_state);
static void show_instructions(const GameResources* res, GameState* game_state);
static void show_settings(const GameResources* res, GameState* game_state);
static void exit_game(const GameResources* res, GameState* game_state);



#endif //HANDLERS_H


