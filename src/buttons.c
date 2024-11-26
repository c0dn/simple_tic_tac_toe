#include <buttons.h>
#include <game.h>
// Button click handlers

/**
 * @brief Function for easiest mode using Naive Bayes model
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When easiest button is clicked, initializes the game with computer enabled
 * and change the game state to Playing
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
 * @brief Function for easy mode using Neural Network model
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When easy button is clicked, initializes the game with computer enabled
 * and change the game state to Playing
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
 * @brief Function for medium mode using Neural Network model
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detailWhen medium button is clicked, initializes the game with computer enabled
 * and change the game state to Playing
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
 * @brief Function for hard mode using Neural Network model
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detailhen hard button is clicked, initializes the game with computer enabled
 * and change the game state to Playing
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
 * @brief Function for continue game button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @details When continue game button is clicked, initializes the game again,
 * All the scores remains
 */
static void continue_playing(const GameResources *res, GameContext *context)
{
    context->transition.start_time = 0;
    context->transition.active = false;
    context->player_1 = context->player_1 == PLAYER_X ? PLAYER_O : PLAYER_X;
    initialize_game(res, context);
}

/**
 * @brief Function for return to menu button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When return to menu button is clicked, reset the score and
 * change state to GAME_STATE_MENU
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
 * @brief Function to exit the game
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When Yes is clicked, close the window
 */
static void exit_game(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->exit_flag = true;
}
/**
 * @brief Function for 1 Player button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When 1 player button is clicked, popup box appears to allow
 * selection of difficulty
 */
static void start_1player(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->state = MENU_DIFF_CHOICE;
}
/**
 * @brief Function for 2 Player button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When 2 player button is clicked, change the game state to playing mode
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
 * @brief Function for how to play button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When how to play button is clicked, go to the instructions page
 */
static void show_instructions(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->state = MENU_INSTRUCTIONS;
}
/**
 * @brief Function for exit button
 * @param context Pointer to the current game context
 * @param res Pointer to the game resources
 * @detail When exit button is clicked, close the game
 */
static void show_exit_confirmation(const GameResources *res, GameContext *context)
{
    PlaySound(res->fx_click);
    context->state = GAME_STATE_EXIT;
}

/**
 * @brief array for GAME_MODE_BUTTONS
 *
 * @param rect define button area
 * @param text define text in the button
 * @param width define width of the button in pixel
 * @param height define height of the button in pixel
 * @param first_render_offset define the vertical offset for the first render
 * @param padding define padding for button (left,right,up,down)
 * @param rounded boolean to determine if button should have rounded corners
 * @param font_size define size for text in button
 * @param action action to execute when button is clicked
 *
 * @details defines buttons for the game modes, when clicked, execute the function accordingly
 */
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

/**
 * @brief array for EXIT_CONFIRMATION_BUTTONS
 *
 * @param rect define button area
 * @param text define text in the button
 * @param width define width of the button in pixel
 * @param height define height of the button in pixel
 * @param first_render_offset define the vertical offset for the first render
 * @param padding define padding for button (left,right,up,down)
 * @param rounded boolean to determine if button should have rounded corners
 * @param font_size define size for text in button
 * @param action action to execute when button is clicked
 *
 * @details defines buttons for the exit confirmation
 * popup box, when clicked, execute the function accordingly
 */
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

/**
 * @brief array for INSTRUCTIONS_BUTTONS
 *
 * @param rect define button area
 * @param text define text in the button
 * @param width define width of the button in pixel
 * @param height define height of the button in pixel
 * @param first_render_offset define the vertical offset for the first render
 * @param padding define padding for button (left,right,up,down)
 * @param rounded boolean to determine if button should have rounded corners
 * @param font_size define size for text in button
 * @param action action to execute when button is clicked
 *
 * @details defines buttons for the instruction page
 * popup box, when clicked, return to main menu
 */
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
/**
 * @brief array for GAME_OVER_BUTTONS
 *
 * @param rect define button area
 * @param text define text in the button
 * @param width define width of the button in pixel
 * @param height define height of the button in pixel
 * @param first_render_offset define the vertical offset for the first render
 * @param padding define padding for button (left,right,up,down)
 * @param rounded boolean to determine if button should have rounded corners
 * @param font_size define size for text in button
 * @param action action to execute when button is clicked
 *
 * @details defines buttons for the popup box in the game when win/lose/draw
 * is decided.
*/
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
/**
 * @brief array for MAIN_MENU_BUTTONS
 *
 * @param rect define button area
 * @param text define text in the button
 * @param width define width of the button in pixel
 * @param height define height of the button in pixel
 * @param first_render_offset define the vertical offset for the first render
 * @param padding define padding for button (left,right,up,down)
 * @param rounded boolean to determine if button should have rounded corners
 * @param font_size define size for text in button
 * @param action action to execute when button is clicked
 *
 * @details defines buttons for the main menu, when clicked, execute the function accordingly */
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
/**
 * @brief array for IN_GAME_BUTTONS
 *
 * @param rect define button area
 * @param text define text in the button
 * @param width define width of the button in pixel
 * @param height define height of the button in pixel
 * @param first_render_offset define the vertical offset for the first render
 * @param padding define padding for button (left,right,up,down)
 * @param rounded boolean to determine if button should have rounded corners
 * @param font_size define size for text in button
 * @param action action to execute when button is clicked
 *
 * @details defines buttons in the game, when clicked, return to main menu*/
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
