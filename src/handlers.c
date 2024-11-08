#include "handlers.h"

#include <buttons.h>
#include <computer.h>
#include <menu.h>
#include <render.h>
#include <stddef.h>

void handle_game_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
{
    const float grid_size = (float)GetScreenWidth() < (float)GetScreenHeight()
                                ? (float)GetScreenWidth() * 0.6f
                                : (float)GetScreenHeight() * 0.6f;
    const int cell_size = (int)grid_size / 3;
    const int start_x = (int)((float)GetScreenWidth() - grid_size) / 2;
    const int start_y = (int)((float)GetScreenHeight() - grid_size) / 2;

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
            context->state = GAME_STATE_DRAW;
            PlaySound(resources->fx_draw);
        }
        else if (check_win(current_player))
        {
            context->state = current_player == PLAYER_X
                                 ? GAME_STATE_P1_WIN
                                 : GAME_STATE_P2_WIN;

            if (!is_computer_win(context))
            {
                PlaySound(resources->fx_win);
            }
        }
        else
        {
            current_player = current_player == PLAYER_X ? PLAYER_O : PLAYER_X;
        }
    }

    if (context->computer_enabled &&
        current_player == get_computer_player(context))
    {
        computer_move(context);
        PlaySound(resources->fx_symbol);

        if (check_draw())
        {
            context->state = GAME_STATE_DRAW;
            PlaySound(resources->fx_draw);
        }
        else if (check_win(PLAYER_O))
        {
            context->state = GAME_STATE_P2_WIN;
            PlaySound(resources->fx_win);
        }
        else
        {
            current_player = PLAYER_X;
        }
    }
}


void handle_exit_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context,
                            bool* exit_flag)
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
                context->state = GAME_STATE_MENU;
                break;
            }
        }
    }
}

void handle_game_mode_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
{
    const size_t button_count = sizeof(GAME_MODE_BUTTONS) / sizeof(Button);
    for (int i = 0; i < button_count; i++)
    {
        const Button btn = GAME_MODE_BUTTONS[i];
        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            btn.action(resources, context);
        }
    }
}

void handle_instructions_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
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
        context->state = GAME_STATE_MENU;
    }
}


void handle_game_over_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
{
    const size_t button_count = sizeof(GAME_OVER_BUTTONS) / sizeof(Button);

    for (int i = 0; i < button_count; i++)
    {
        const Button btn = GAME_OVER_BUTTONS[i];

        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            btn.action(resources, context);
        }
    }
}

void handle_music_toggle(const GameResources* resources, GameContext* context)
{
    context->audio_disabled = !context->audio_disabled;

    if (context->audio_disabled)
    {
        StopMusicStream(resources->background_music);
    }
    else
    {
        PlayMusicStream(resources->background_music);
    }
}

void handle_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
{
    const size_t button_count = sizeof(MAIN_MENU_BUTTONS) / sizeof(Button);
    const Rectangle audio_ico_rect = calc_music_icon_rect(context, resources);
    if (CheckCollisionPointRec(mouse_pos, audio_ico_rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        handle_music_toggle(resources, context);
    }

    for (int i = 0; i < button_count; i++)
    {
        const Button btn = MAIN_MENU_BUTTONS[i];

        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            btn.action(resources, context);
        }
    }
}
