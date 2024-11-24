#include "handlers.h"

#include <buttons.h>
#include <computer.h>
#include <render.h>

/**
 * @brief Handle mouse clicks during active game play
 *
 * Manages player moves, game state transitions, and computer turns
 *
 * @param click_pos Click position
 * @param resources Game asset resources
 * @param context Current game context
 * @param buttons Button array
 * @param count Number of buttons in array
 */
void handle_clicks(const Vector2 click_pos, const GameResources* resources, GameContext* context, const Button* buttons, const size_t count)
{
    for (int i = 0; i < count; i++)
    {
        const Button btn = buttons[i];
        if (btn.rect)
        {
            if (CheckCollisionPointRec(click_pos, *btn.rect) &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                btn.action(resources, context);
            }
        }
    }
}


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
    const int row = ((int)mouse_pos.y - context->grid.start_y) / context->grid.cell_size;
    const int col = ((int)mouse_pos.x - context->grid.start_x) / context->grid.cell_size;

    // Check if click is within board and cell is empty
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && is_cell_empty(row, col))
    {
        PlaySound(resources->fx_symbol);
        set_cell(row, col, current_player);

        // Update game state score after player move
        update_game_state_score(context);

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

            // Update game state score after computer move
            update_game_state_score(context);

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
    }
    const Rectangle audio_ico_rect = calc_music_icon_rect(context, resources);
    if (CheckCollisionPointRec(mouse_pos, audio_ico_rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        handle_music_toggle(resources, context);
    }
    else
    {
        handle_clicks(mouse_pos, resources, context, IN_GAME_BUTTONS, 1);
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
    const Rectangle audio_ico_rect = calc_music_icon_rect(context, resources);
    if (CheckCollisionPointRec(mouse_pos, audio_ico_rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        handle_music_toggle(resources, context);
    } else
    {
        handle_clicks(mouse_pos, resources, context, MAIN_MENU_BUTTONS, 4);
    }
}
