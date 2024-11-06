#include "handlers.h"

#include <menu.h>
#include <render.h>


void handle_game_click(const Vector2 mouse_pos, const float screen_width, const float screen_height,
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
    initialize_game(res, game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void start_medium_mode(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void start_hard_mode(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
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

static void start_1player(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    *game_state = MENU_DIFF_CHOICE;
}

static void start_2player(const GameResources* res, GameState* game_state) {
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
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


void handle_settings_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state) {
    static const float btn_width = 330.0f;
    static const float btn_height = 100.0f;
    static const float FIRST_BUTTON_OFFSET = -200.0f;
    static const int BUTTON_SPACING = 130;

    const float start_y = (float)GetScreenHeight() / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;

    for (int i = 0; i < 4; i++) {
        const Rectangle btnRect = {
            (float)GetScreenWidth() / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };

        if (CheckCollisionPointRec(mouse_pos, btnRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(resources->fx_click);
            switch(i) {
                case 0: // Play
                    if (!IsMusicStreamPlaying(resources->background_music)) {
                        PlayMusicStream(resources->background_music);
                    }
                    break;
                case 1: // Pause
                    if (IsMusicStreamPlaying(resources->background_music)) {
                        PauseMusicStream(resources->background_music);
                    }
                    break;
                case 2: // Stop
                    if (IsMusicStreamPlaying(resources->background_music)) {
                        StopMusicStream(resources->background_music);
                    }
                    break;
                case 3: // Back
                    *state = GAME_STATE_MENU;
                    break;
            }
        }
    }
}

void handle_exit_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state) {
    static const float btn_width = 330.0f;
    static const float btn_height = 100.0f;
    static const float FIRST_BUTTON_OFFSET = -40.0f;
    static const int BUTTON_SPACING = 130;

    const Button buttons[] = {
        { .action = close_window },  // Yes
        { .action = return_to_menu } // No
    };

    const float start_y = (float)GetScreenHeight() / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;

    for (int i = 0; i < 2; i++) {
        const Rectangle btnRect = {
            (float)GetScreenWidth() / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };

        if (CheckCollisionPointRec(mouse_pos, btnRect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            buttons[i].action) {
            buttons[i].action(resources, state);
        }
    }
}

void handle_game_mode_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state) {
    static const float btn_width = 330.0f;
    static const float btn_height = 100.0f;
    static const float FIRST_BUTTON_OFFSET = -90.0f;
    static const int BUTTON_SPACING = 130;

    const Button buttons[] = {
        { .action = start_easy_mode },
        { .action = start_medium_mode },
        { .action = start_hard_mode }
    };

    const float start_y = (float)GetScreenHeight() / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;

    for (int i = 0; i < 3; i++) {
        const Rectangle btnRect = {
            (float)GetScreenWidth() / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };

        if (CheckCollisionPointRec(mouse_pos, btnRect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            buttons[i].action) {
            buttons[i].action(resources, state);
        }
    }
}

void handle_instructions_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state) {
    static const float btn_width = 330.0f;
    static const float btn_height = 100.0f;
    static const float FIRST_BUTTON_OFFSET = 300.0f;

    const Rectangle btnRect = {
        (float)GetScreenWidth() / 2.0f - btn_width / 2.0f,
        (float)GetScreenHeight() / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET,
        btn_width,
        btn_height
    };

    if (CheckCollisionPointRec(mouse_pos, btnRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_MENU;
        }
}


void handle_game_over_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state) {
    static const float BUTTON_SPACING = 50.0f;
    static const float btn_width = 330.0f;
    static const float btn_height = 100.0f;

    const float box_width = (float)GetScreenWidth() * 0.7f;
    const float box_height = (float)GetScreenHeight() * 0.4f;
    const float box_x = ((float)GetScreenWidth() - box_width) / 2;
    const float box_y = ((float)GetScreenHeight() - box_height) / 2;

    const Button buttons[] = {
        { .action = initialize_game },  // Restart
        { .action = return_to_menu }   // Back to a menu
    };

    // Calculate button positions - matching render_game_over
    const float total_buttons_width = (btn_width * 2) + BUTTON_SPACING;
    const float start_x = box_x + (box_width - total_buttons_width) / 2;
    const float buttons_y = box_y + box_height - btn_height - 40;

    for (int i = 0; i < 2; i++) {
        const Rectangle btnRect = {
            start_x + (btn_width + BUTTON_SPACING) * i,
            buttons_y,
            btn_width,
            btn_height
        };

        if (CheckCollisionPointRec(mouse_pos, btnRect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            buttons[i].action) {
            buttons[i].action(resources, state);
            }
    }
}

void handle_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state) {
    static const float btn_width = 330.0f;
    static const float btn_height = 100.0f;
    static const float FIRST_BUTTON_OFFSET = -300.0f;
    static const int BUTTON_SPACING = 130;

    const Button buttons[] = {
        { .text = "1 Player", .action = start_1player },
        { .text = "2 Player", .action = start_2player },
        { .text = "How To Play", .action = show_instructions },
        { .text = "Settings", .action = show_settings },
        { .text = "Exit", .action = exit_game }
    };

    const float start_y = (float)GetScreenHeight() / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;

    for (int i = 0; i < 5; i++) {
        const Rectangle btnRect = {
            (float)GetScreenWidth() / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };

        if (CheckCollisionPointRec(mouse_pos, btnRect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            buttons[i].action) {
            buttons[i].action(resources, state);
            }
    }
}
