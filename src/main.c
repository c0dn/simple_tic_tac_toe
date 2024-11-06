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

    InitWindow((int)screen_width, (int)screen_height, "Tic Tae Toe");
    SetWindowState(FLAG_VSYNC_HINT);
    InitAudioDevice();

    const GameResources resources = load_game_resources((int)screen_width, (int)screen_height);

    SetTargetFPS(60);
    PlayMusicStream(resources.background_music);
    GameState current_game_state = GAME_STATE_MENU;
    bool exit_flag = false;
    while (!exit_flag) // Detect window close button
    {
        UpdateMusicStream(resources.background_music);
        const Vector2 mouse_pos = GetMousePosition();
        // Updates
        if (WindowShouldClose()) exit_flag = true;
        switch (current_game_state)
        {
        case GAME_STATE_MENU:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                handle_menu_click(mouse_pos, &resources, &current_game_state);
            }
            break;
        case GAME_STATE_PLAYING:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {

                handle_game_click(mouse_pos, screen_width, screen_height,
                &resources, &current_game_state);
            }

            break;

        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                handle_game_over_menu_click(mouse_pos, &resources, &current_game_state);
            }
            break;

        case MENU_SETTINGS:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                handle_settings_menu_click(mouse_pos, &resources, &current_game_state);
            }
            break;

        case GAME_STATE_EXIT:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                handle_exit_menu_click(mouse_pos, &resources, &current_game_state, &exit_flag);
            }
            break;

        case MENU_DIFF_CHOICE:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                handle_game_mode_menu_click(mouse_pos, &resources, &current_game_state);
            }
            break;
        case MENU_INSTRUCTIONS:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                handle_instructions_menu_click(mouse_pos, &resources, &current_game_state);
            }
            break;
        default:
            break;
        }
        // Drawing
        BeginDrawing();
        ClearBackground(LIME);
        switch (current_game_state)
        {
        case GAME_STATE_MENU:
            render_menu(&resources);
            break;
        case GAME_STATE_PLAYING:
            render_grid(&resources);
            break;
        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
            render_grid(&resources);
            render_game_over(&current_game_state);
            break;

        case MENU_INSTRUCTIONS:
            render_instructions(&resources);
            break;

        case MENU_SETTINGS:
            render_settings();
            break;

        case GAME_STATE_EXIT:
            render_exit();
            break;

        case MENU_DIFF_CHOICE:
            render_game_mode_choice();
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
