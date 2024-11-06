#include "handlers.h"

#include <menu.h>


void handle_mouse_click(const Vector2 mouse_pos, const float screen_width, const float screen_height,
    const GameResources* resources, GameState* state)
{
    const float grid_size = screen_width < screen_height ? screen_width * 0.6f : screen_height * 0.6f;
    const int cell_size = (int)grid_size / 3;
    const int start_x = (int)(screen_width - grid_size) / 2;
    const int start_y = (int)(screen_height - grid_size) / 2;

    // Convert mouse position to board coordinates
    const int row = ((int)mouse_pos.y - start_y) / cell_size;
    const int col = ((int)mouse_pos.x - start_x) / cell_size;

    // Check if click is within board and cell is empty
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && is_cell_empty(row, col))
    {
        PlaySound(resources->fx_symbol);
        set_cell(row, col, current_player);

        if (check_draw())
        {
            *state = GAME_STATE_DRAW;
            PlaySound(resources->fx_draw);
        }
        else if (check_win(current_player))
        {
            *state = current_player == PLAYER_X ? GAME_STATE_P1_WIN : GAME_STATE_P2_WIN;
            PlaySound(resources->fx_win);
        }
        else
        {
            current_player = current_player == PLAYER_X ? PLAYER_O : PLAYER_X;
        }
    }
}

// Button click handlers
static void start_easy_mode(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void start_medium_mode(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void start_hard_mode(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(game_state);
    *game_state = GAME_STATE_PLAYING;
}


static void close_window(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = GAME_STATE_EXIT;
}

static void return_to_menu(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = GAME_STATE_MENU;
}

// Button definitions with action functions
static void start_1player(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = MENU_DIFF_CHOICE;
}

static void start_2player(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void show_instructions(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = MENU_INSTRUCTIONS;
}

static void show_settings(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = MENU_SETTINGS;
}

static void exit_game(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = GAME_STATE_EXIT;
}
