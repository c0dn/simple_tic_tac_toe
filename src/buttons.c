#include <buttons.h>
#include <game.h>
// Button click handlers

/**
 * @brief Click handler for easy mode selection (Naive bayes)
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void start_easy_mode_naive(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_EASY_NAIVE;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

/**
 * @brief Click handler for easy mode selection (neural_net)
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void start_easy_mode_NN(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_EASY_NN;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

/**
 * @brief Click handler for medium mode selection
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void start_medium_mode(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_MEDIUM;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

/**
 * @brief Click handler for hard mode selection
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void start_hard_mode(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_HARD;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

/**
 * @brief Click handler for "continue" button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @details Starts another game with previous settings, scores are preserved
 */
static void continue_playing(const GameResources *res, GameContext *context)
{
    context->transition.start_time = 0;
    context->transition.active = false;
    context->player_1 = context->player_1 == PLAYER_X ? PLAYER_O : PLAYER_X;
    initialize_game(res, context);
}

/**
 * @brief Click handler for returning to menu
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail Resets scores, set state to menu etc.
 */
static void return_to_menu(const GameResources *res, GameContext *context)
{
    context->transition.start_time = 0;
    context->transition.active = false;
    PlaySound(res->fx_click);
    context->p1_score = 0;
    context->p2_score = 0;
    context->draw_score = 0;

    context->state = GAME_STATE_MENU;
}
/**
 * @brief Click handler for exit confirmation "yes"
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void exit_game(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->exit_flag = true;
}
/**
 * @brief Click handler for "1 Player" button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void start_1player(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->state = MENU_DIFF_CHOICE;
}
/**
 * @brief Click handler for "2 Player" button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void start_2player(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = TWO_PLAYER;
    context->computer_enabled = false;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}
/**
 * @brief Click handler for "how to play" button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void show_instructions(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->state = MENU_INSTRUCTIONS;
}
/**
 * @brief Click handler for the exit button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 */
static void show_exit_confirmation(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->state = GAME_STATE_EXIT;
}


Button GAME_MODE_BUTTONS[] = {
    {.rect = NULL,
     .text = "Easiest",
     .width = 300.0f,
     .height = 50.0f,
     .first_render_offset = -75.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .rounded = true,
     .font_size = 30,
     .action = start_easy_mode_naive},
    {.rect = NULL,
     .text = "Easy",
     .width = 300.0f,
     .height = 50.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .first_render_offset = -75.0f,
     .rounded = true,
     .font_size = 30,
     .action = start_easy_mode_NN},
    {.rect = NULL,
     .text = "Medium",
     .width = 300.0f,
     .height = 50.0f,
     .first_render_offset = -75.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .rounded = true,
     .font_size = 30,
     .action = start_medium_mode},
    {.rect = NULL,
     .text = "Hard",
     .width = 300.0f,
     .height = 50.0f,
     .first_render_offset = -75.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .rounded = true,
     .font_size = 30,
     .action = start_hard_mode}};


Button EXIT_CONFIRMATION_BUTTONS[] = {
    {.rect = NULL,
     .text = "Yes",
     .width = 330.0f,
     .height = 100.0f,
     .first_render_offset = -40.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .rounded = true,
     .font_size = 30,
     .action = exit_game},
    {.rect = NULL,
     .text = "No",
     .width = 330.0f,
     .height = 100.0f,
     .first_render_offset = -40.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .rounded = true,
     .font_size = 30,
     .action = return_to_menu}};


Button INSTRUCTIONS_BUTTONS[] = {
    {.rect = NULL,
     .text = "Back",
     .width = 330.0f,
     .height = 100.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .first_render_offset = 300.0f,
     .rounded = true,
     .font_size = 70,
     .action = return_to_menu}};


Button GAME_OVER_BUTTONS[] = {
    {.rect = NULL,
     .text = "Continue Playing",
     .width = 200,
     .height = 60,
     .first_render_offset = 0,
     .padding = {0, 0, 15.0f, 15.0f},
     .rounded = true,
     .font_size = 20,
     .action = continue_playing},
    {.rect = NULL,
     .text = "End Game",
     .width = 200,
     .height = 60,
     .first_render_offset = 0,
     .padding = {0, 0, 15.0f, 15.0f},
     .rounded = true,
     .font_size = 20,
     .action = return_to_menu}};


Button MAIN_MENU_BUTTONS[] = {
    {.rect = NULL,
     .text = "1 Player",
     .width = 200.0f,
     .height = 60.0f,
     .first_render_offset = 100.0f,
     .padding = {20, 20, 10, 10},
     .rounded = true,
     .font_size = 30,
     .action = start_1player},
    {.rect = NULL,
     .text = "2 Player",
     .width = 200.0f,
     .height = 60.0f,
     .first_render_offset = 100.0f,
     .padding = {20, 20, 10, 10},
     .rounded = true,
     .font_size = 30,
     .action = start_2player},
    {.rect = NULL,
     .text = "How To Play",
     .width = 200.0f,
     .height = 60.0f,
     .first_render_offset = 120.0f,
     .padding = {20, 20, 10, 10},
     .rounded = true,
     .font_size = 30,
     .action = show_instructions},
    {.rect = NULL,
     .text = "Exit",
     .width = 200.0f,
     .height = 60.0f,
     .first_render_offset = 120.0f,
     .padding = {20, 20, 10, 10},
     .rounded = true,
     .font_size = 30,
     .action = show_exit_confirmation}};


Button IN_GAME_BUTTONS[] = {
    {.rect = NULL,
     .text = "Return to menu",
     .width = 330.0f,
     .height = 100.0f,
     .padding = {0, 0, 10.0f, 10.0f},
     .first_render_offset = 400.0f,
     .rounded = true,
     .font_size = 40,
     .action = return_to_menu}};
