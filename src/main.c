/**
* @file main.c
 * @brief Main entry point for the Tic-Tac-Toe game, game created using raylib
 *
 * This file contains the main game loop, initialization, and event handling
 * for the Tic-Tac-Toe game implemented with a text-based user interface.
 */

#include <game.h>
#include <render.h>
#include <handlers.h>
#include <menu.h>
#include <raylib.h>
#include <stdlib.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const float screen_width = 1000;
    const float screen_height = 1000;

    GameContext context = {
        .needs_redraw = true,
        .state = GAME_STATE_MENU,
        .selected_game_mode = TWO_PLAYER,
        .player_1 = PLAYER_X,
        .computer_enabled = false,
        .audio_disabled = false,
    };

    const UiOptions render_options = {
        .background_color = { 226, 232, 240, 255 },
        .btn_clicked_color = ORANGE,
        .primary_btn_color = GOLD
    };

    InitWindow((int)screen_width, (int)screen_height, "Tic Tae Toe");
    InitAudioDevice();

    const GameResources resources = load_game_resources((int)screen_width, (int)screen_height);

    SetTargetFPS(60);
    PlayMusicStream(resources.background_music);
    bool exit_flag = false;
    while (!exit_flag) // Detect window close button
    {
        UpdateMusicStream(resources.background_music);
        // Updates
        const Vector2 mouse_pos = GetMousePosition();
        if (WindowShouldClose()) exit_flag = true;
        switch (context.state)
        {
        case GAME_STATE_MENU:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                context.needs_redraw = true;
                handle_menu_click(mouse_pos, &resources, &context);
            }
            break;
        case GAME_STATE_PLAYING:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP))
            {
                context.needs_redraw = true;
                handle_game_click(mouse_pos, &resources, &context);
            }
            break;
        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                context.needs_redraw = true;
                handle_game_over_menu_click(mouse_pos, &resources, &context);
            }
            break;


        case GAME_STATE_EXIT:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                context.needs_redraw = true;
                handle_exit_menu_click(mouse_pos, &resources, &context, &exit_flag);
            }
            break;

        case MENU_DIFF_CHOICE:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                context.needs_redraw = true;
                handle_game_mode_menu_click(mouse_pos, &resources, &context);
            }
            break;
        case MENU_INSTRUCTIONS:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                context.needs_redraw = true;
                handle_instructions_menu_click(mouse_pos, &resources, &context);
            }
            break;
        default:
            break;
        }
        BeginDrawing();
        switch (context.state)
        {
        case GAME_STATE_MENU:
            render_menu(&resources, &render_options, &context);
            break;
        case GAME_STATE_PLAYING:
            render_grid(&resources, &render_options, &context);
            break;
        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
            render_grid(&resources, &render_options,&context);
            render_game_over(&context, &render_options);
            display_score(&context);
            break;

        case MENU_INSTRUCTIONS:
            render_instructions(&resources, &render_options);
            break;

        case GAME_STATE_EXIT:
            render_exit(&render_options);
            break;

        case MENU_DIFF_CHOICE:
            render_game_mode_choice(&render_options);
            break;

        default:
            break;
        }
        EndDrawing();
    }
    unload_game_resources(&resources);
    CloseAudioDevice();
    CloseWindow();
    return EXIT_SUCCESS;
}
