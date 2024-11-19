#include "handlers.h"

#include <buttons.h>
#include <computer.h>
#include <render.h>
#include <stddef.h>

/**
 * @brief Handle mouse clicks during active game play
 *
 * Manages player moves, game state transitions, and computer turns
 *
 * @param mouse_pos Current mouse position
 * @param resources Game asset resources
 * @param context Current game context
 */
void handle_game_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    const float grid_size = (float)screen_width < (float)screen_height
                                ? (float)screen_width * 0.6f
                                : (float)screen_height * 0.6f;
    const int cell_size = (int)grid_size / 3;
    const int start_x = (int)((float)screen_width - grid_size) / 2;
    const int start_y = (int)((float)screen_height - grid_size) / 2;

    // Convert mouse position to board coordinates
    const int row = ((int)mouse_pos.y - start_y) / cell_size;
    const int col = ((int)mouse_pos.x - start_x) / cell_size;

    // Check if click is within board and cell is empty
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && is_cell_empty(row, col))
    {
        PlaySound(resources->fx_symbol);
        set_cell(row, col, current_player);

        // Update game state and score after player's move
        update_game_state_and_score(context);

        // Play specific sounds based on game state
        if (context->state == GAME_STATE_DRAW)
        {
            PlaySound(resources->fx_draw);
        }
        else if (context->state == GAME_STATE_P1_WIN || context->state == GAME_STATE_P2_WIN)
        {
            if (!is_computer_win(context))
            {
                PlaySound(resources->fx_win);
            }
        }
        else
        {
            // Toggle to the next player if the game is still ongoing
            current_player = current_player == PLAYER_X ? PLAYER_O : PLAYER_X;
        }
        // Handle computer move if enabled and it's the computer's turn
        if (context->computer_enabled &&
            current_player == get_computer_player(context))
        {
            computer_move(context, resources->models);
            PlaySound(resources->fx_symbol);

            // Update game state and score after computer's move
            update_game_state_and_score(context);

            // Play specific sounds based on game state
            if (context->state == GAME_STATE_DRAW)
            {
                PlaySound(resources->fx_draw);
            }
            else if (context->state == GAME_STATE_P2_WIN)
            {
                PlaySound(resources->fx_win);
            }
            else
            {
                // Toggle back to Player X if the game is still ongoing
                current_player = current_player == PLAYER_X ? PLAYER_O : PLAYER_X;
            }
        }
    } else
    {
        const size_t button_count = sizeof(IN_GAME_BUTTONS) / sizeof(Button);

        for (int i = 0; i < button_count; i++)
        {
            const Button btn = IN_GAME_BUTTONS[i];
            if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btn.action(resources, context);
            }
        }
    }
}

/**
 * @brief Handle clicks in the exit confirmation menu
 *
 * @param mouse_pos Current mouse position
 * @param resources Game asset resources
 * @param context Current game context
 * @param exit_flag Pointer to flag controlling game exit
 */
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

/**
 * @brief Handle game mode selection menu interactions
 *
 * @param mouse_pos Current mouse position
 * @param resources Game asset resources
 * @param context Current game context
 */
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

/**
 * @brief Handle clicks in the instructions menu
 *
 * @param mouse_pos Current mouse position
 * @param resources Game asset resources
 * @param context Current game context
 */
void handle_instructions_menu_click(const Vector2 mouse_pos, const GameResources* resources, GameContext* context)
{
    const size_t button_count = sizeof(INSTRUCTIONS_BUTTONS) / sizeof(Button);
    for (int i = 0; i < button_count; i++)
    {
        const Button btn = INSTRUCTIONS_BUTTONS[i];
        if (CheckCollisionPointRec(mouse_pos, btn.rect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            btn.action)
        {
            PlaySound(resources->fx_click);
            btn.action(resources, context);
        }
    }
}


/**
 * @brief Handle interactions in game over screen
 *
 * @param mouse_pos Current mouse position
 * @param resources Game asset resources
 * @param context Current game context
 */
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
            context->transition.start_time = 0;
            context->transition.active = false;
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

/**
 * @brief Handle main menu interactions
 *
 * @param mouse_pos Current mouse position
 * @param resources Game asset resources
 * @param context Current game context
 */
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
