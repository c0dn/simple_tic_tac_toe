#include "handlers.h"

#include <buttons.h>
#include <menu.h>
#include <render.h>
#include <stddef.h>


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

void handle_settings_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state)
{
    const size_t button_count = sizeof(SETTINGS_BUTTONS) / sizeof(Button);
    for (int i = 0; i < button_count; i++)
    {
        if (CheckCollisionPointRec(mouse_pos, SETTINGS_BUTTONS[i].rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(resources->fx_click);
            switch (i)
            {
            case 0: // Play
                if (!IsMusicStreamPlaying(resources->background_music))
                {
                    PlayMusicStream(resources->background_music);
                }
                break;
            case 1: // Pause
                if (IsMusicStreamPlaying(resources->background_music))
                {
                    PauseMusicStream(resources->background_music);
                }
                break;
            case 2: // Stop
                if (IsMusicStreamPlaying(resources->background_music))
                {
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

void handle_exit_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state, bool* exit_flag)
{
    const size_t button_count = sizeof(EXIT_CONFIRMATION_BUTTONS) / sizeof(Button);


    for (int i = 0; i < button_count; i++)
    {
        const Button btn = EXIT_CONFIRMATION_BUTTONS[i];
        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(resources->fx_click);
            switch (i)
            {
            case 0: // Close window
                *exit_flag = true;
                break;
            case 1: // Back to main menu
                *state = GAME_STATE_MENU;
                break;
            }
        }
    }
}

void handle_game_mode_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state)
{
    const size_t button_count = sizeof(GAME_MODE_BUTTONS) / sizeof(Button);
    for (int i = 0; i < button_count; i++)
    {
        const Button btn = GAME_MODE_BUTTONS[i];
        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            btn.action(resources, state);
        }
    }
}

void handle_instructions_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state)
{
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
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_MENU;
    }
}


void handle_game_over_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state)
{
    const size_t button_count = sizeof(GAME_OVER_BUTTONS) / sizeof(Button);

    for (int i = 0; i < button_count; i++)
    {
        const Button btn = GAME_OVER_BUTTONS[i];

        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            btn.action(resources, state);
        }
    }
}

void handle_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameState* state)
{
    const size_t button_count = sizeof(MAIN_MENU_BUTTONS) / sizeof(Button);

    for (int i = 0; i < button_count; i++)
    {
        const Button btn = MAIN_MENU_BUTTONS[i];

        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            btn.action(resources, state);
        }
    }
}
