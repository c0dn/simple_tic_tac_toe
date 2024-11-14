#ifndef HANDLERS_H
#define HANDLERS_H
#include <game.h>
#include <menu.h>
#include <raylib.h>

void handle_game_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);

void handle_menu_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);
void handle_exit_menu_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context, bool* exit_flag);
void handle_game_mode_menu_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);
void handle_instructions_menu_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);
void handle_game_over_menu_click(Vector2 mouse_pos, const GameResources* resources, GameContext* context);
void handle_music_toggle(const GameResources* resources, GameContext* context);

void handle_game_over_transition(
    const GameResources* resources,
    const UiOptions* render_opts,
    GameContext* context,
    Vector2 mouse_pos
);


#endif //HANDLERS_H