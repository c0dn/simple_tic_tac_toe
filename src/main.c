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
#include <raylib.h>
#include <stdlib.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screen_width = 800;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "Tic Tae Toe");

    SetTargetFPS(60);
    GameState current_game_state = GAME_STATE_MENU;
    while (!WindowShouldClose()) // Detect window close button
    {
        // Updates
        switch (current_game_state)
        {
        case GAME_STATE_MENU:
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                initialize_game(&current_game_state);
            }
        case GAME_STATE_PLAYING:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                const Vector2 mouse_pos = GetMousePosition();
                handle_mouse_click(mouse_pos, screen_width, screen_height, &current_game_state);
            }

            break;

        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
            if (IsKeyPressed(KEY_ENTER))
            {
                initialize_game(&current_game_state);
            }
            break;

        default:
            break;
        }
        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        switch (current_game_state)
        {
        case GAME_STATE_MENU:
            render_menu(screen_height, screen_width);
            break;
        case GAME_STATE_PLAYING:
            render_grid(screen_height, screen_width);
            break;
        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
            render_grid(screen_height, screen_width);
            render_game_over(screen_height, screen_width, current_game_state);
            break;

        default:
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
