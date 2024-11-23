/**
 * @file main.c
 * @brief Main entry point for the Tic-Tac-Toe game, game created using raylib
 *
 * This file contains the main game loop, initialization, and event handling
 * for the Tic-Tac-Toe game implemented with a text-based user interface.
 */

#include <render.h>
#include <handlers.h>
#include <memo.h>
#include <menu.h>
#include <raylib.h>
#include <stdlib.h>
#include <uthash.h>
#include <computer.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const float screen_width = 1000;
    const float screen_height = 1000;

    MemoCache *memo_cache = init_memo_cache();
    if (!memo_cache)
    {
        TraceLog(LOG_ERROR, "Failed to initialize memo cache\n");
        return EXIT_FAILURE;
    }

    GridDimensions default_grid = {0};

    GameContext context = {
        .needs_recalculation = false,
        .exit_flag = false,
        .grid = default_grid,
        .state = GAME_STATE_MENU,
        .selected_game_mode = TWO_PLAYER,
        .player_1 = PLAYER_X,
        .computer_enabled = false,
        .both_computers_enabled = true,
        .audio_disabled = false,
        .transition = {
            .start_time = 0,
            .active = false},
        .start_screen_shown = false,
        .p1_score = 0,
        .p2_score = 0,
        .memo_cache = memo_cache,
    };

    const UiOptions render_options = {
        .background_color = {226, 232, 240, 255},
        .btn_clicked_color = ORANGE,
        .primary_btn_color = GOLD};

    InitWindow((int)screen_width, (int)screen_height, "Tic Tae Toe");
    InitAudioDevice();

    GameResources resources = load_game_resources();

    update_grid_dimensions(&context);

    SetTargetFPS(60);
    PlayMusicStream(resources.background_music);
    while (!context.exit_flag) // Detect window close button
    {
        if (IsWindowResized())
        {
            context.needs_recalculation = true;
            update_grid_dimensions(&context);
        }
        UpdateMusicStream(resources.background_music);
        // Updates
        const Vector2 mouse_pos = GetMousePosition();
        if (WindowShouldClose())
            context.exit_flag = true;
        switch (context.state)
        {
        case GAME_STATE_MENU:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                handle_menu_click(mouse_pos, &resources, &context);
            }
            break;
        case GAME_STATE_PLAYING:
            if (context.both_computers_enabled)
            {
                nn_vs_nb(&context, &resources.models);
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && context.start_screen_shown)
            {
                handle_game_click(mouse_pos, &resources, &context);
            }
            break;
        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
        case GAME_STATE_NN_WIN:
        case GAME_STATE_NB_WIN:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                handle_clicks(mouse_pos, &resources, &context, GAME_OVER_BUTTONS, 2);
            }
            break;

        case GAME_STATE_EXIT:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                handle_clicks(mouse_pos, &resources, &context, EXIT_CONFIRMATION_BUTTONS, 2);
            }
            break;

        case MENU_DIFF_CHOICE:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                handle_clicks(mouse_pos, &resources, &context, GAME_MODE_BUTTONS, 5);
            }
            break;
        case MENU_INSTRUCTIONS:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                handle_clicks(mouse_pos, &resources, &context, INSTRUCTIONS_BUTTONS, 1);
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
            if (!context.start_screen_shown)
            {
                do_game_start_transition(&resources, &render_options, &context);
            }
            else
            {
                render_grid(&resources, &render_options, &context, true);
            }
            break;
        case GAME_STATE_P1_WIN:
        case GAME_STATE_P2_WIN:
        case GAME_STATE_DRAW:
        case GAME_STATE_NN_WIN:
        case GAME_STATE_NB_WIN:
            do_game_over_transition(&resources, &render_options, &context);
            break;

        case MENU_INSTRUCTIONS:
            render_instructions(&resources, &render_options, &context);
            break;

        case GAME_STATE_EXIT:
            render_exit(&render_options, &context);
            break;

        case MENU_DIFF_CHOICE:
            render_game_mode_choice(&render_options, &context);
            break;

        default:
            break;
        }
        EndDrawing();
    }
    unload_game_resources(&resources);
    cleanup_memo_cache(context.memo_cache);
    CloseAudioDevice();
    CloseWindow();
    return EXIT_SUCCESS;
}
