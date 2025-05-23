#include "render.h"
#include <buttons.h>
#include <computer.h>

#include <raylib.h>
#include <utils.h>

/**
 * @brief Renders buttons onto the window
 * @param buttons Pointer to Button array
 * @param button_count size of the Button array passed to function
 * @param buttons_per_row number of buttons on a row
 * @param render_opts Pointer to UiOptions
 * @param cache Pointer to a MemoCache
 * @param need_recalc boolean to indicate whether cached Rectangles need to be recalculated
 */
static void render_buttons(Button* buttons, const size_t button_count, const int buttons_per_row,
                           const UiOptions* render_opts, MemoCache* cache, const bool need_recalc)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    for (int i = 0; i < button_count; i++)
    {
        if (buttons[i].rect == NULL || need_recalc)
        {
            if (buttons[i].rect == NULL)
            {
                buttons[i].rect = (Rectangle*)malloc(sizeof(Rectangle));
            }
            *buttons[i].rect = calculate_button_rectangle(buttons[i].width, buttons[i].padding, buttons[i].height,
                                                          buttons[i].first_render_offset, i, buttons_per_row,
                                                          screen_height, screen_width);
        }
        const bool overwrite_default_colors = buttons[i].override_default_colors;
        const bool is_hovering = CheckCollisionPointRec(GetMousePosition(), *buttons[i].rect);
        const Color buttonColor = is_hovering
                                      ? (overwrite_default_colors
                                             ? buttons[i].clickColor
                                             : render_opts->btn_clicked_color)
                                      : overwrite_default_colors
                                      ? buttons[i].color
                                      : render_opts->primary_btn_color;


        // Draw button
        if (buttons[i].rounded)
        {
            DrawRectangleRounded(*buttons[i].rect, 0.3f, 20, buttonColor);
            DrawRectangleRoundedLines(*buttons[i].rect, 0.3f, 20, 2.0f, GRAY);
        }
        else
        {
            DrawRectangleRounded(*buttons[i].rect, 0, 20, buttonColor);
            DrawRectangleRoundedLines(*buttons[i].rect, 0, 20, 2.0f, GRAY);
        }

        // Center text
        const Coords cords =
            calculate_centered_text_xy(buttons[i].text, buttons[i].font_size, buttons[i].rect->x, buttons[i].rect->y,
                                       buttons[i].rect->width, buttons[i].rect->height);

        DrawText(buttons[i].text, (int)cords.x, (int)cords.y, buttons[i].font_size, BLACK);
    }
}

/**
 * @brief Function for game start transition
 * @param resources Pointer to the GameResources
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 * @details Manage how the game transitions at the start of game.
 * Display message showing which player begins first
 *
 */
void do_game_start_transition(const GameResources* resources, const UiOptions* render_opts, GameContext* context)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    if (!context->transition.active)
    {
        context->transition.start_time = GetTime();
        context->transition.active = true;
    }

    render_grid(resources, render_opts, context, false);

    const double elapsed_time = GetTime() - context->transition.start_time;

    // Semi-transparent background
    DrawRectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 100});

    const char* start_msg = context->computer_enabled
                                ? (current_player == get_human_player(context)
                                       ? "Player starts first"
                                       : "Computer starts first")
                                : current_player == context->player_1
                                ? "Player 1 starts first"
                                : "Player 2 starts first";

    // Calculate centered text position
    const Coords text_coords =
        calculate_centered_text_xy(start_msg, 40, 0, 0, (float)screen_width, (float)screen_height);

    DrawText(start_msg, (int)text_coords.x, (int)text_coords.y, 40, RAYWHITE);

    // Reset transition after 1 second
    if (elapsed_time >= 1.0)
    {
        context->transition.active = false;
        context->start_screen_shown = true;
        if (current_player == get_computer_player(context) && context->computer_enabled)
        {
            computer_move(context, resources->models);
            PlaySound(resources->fx_symbol);
            current_player = get_computer_player(context) == PLAYER_X ? PLAYER_O : PLAYER_X;
        }
    }
}

/**
 * @brief Returns a string representing the name of the game mode
 * @param mode Pointer to a GameMode and it is constant
 * @return string constant that indicates the game modes
 */
const char* get_game_mode_name(const GameMode* mode) {
    switch (*mode) {
        case ONE_PLAYER_EASY_NN:
            return "Easy Mode (Neural Network)";
        case ONE_PLAYER_EASY_NAIVE:
            return "Easy Mode (Naive Bayes)";
        case ONE_PLAYER_MEDIUM:
            return "Medium Mode (Imperfect Minimax)";
        case ONE_PLAYER_HARD:
            return "Hard Mode (Perfect Minimax)";
        default:
            return "Two Player Mode";
    }
}

/**
 * @brief Renders the game grid, game mode, current player symbols, and optional UI elements
 * @param resources Pointer to the GameResources
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 * @param show_buttons boolean to indicate if buttons should be rendered or not
 * @details This function renders the game grid with player symbols
 * - Render music toggle button.
 * - If the boolean show_buttons is true, renders return to menu button.
 * - Draw winning grid lines if there is a winner
 */
void render_grid(const GameResources* resources, const UiOptions* render_opts, const GameContext* context,
                 const bool show_buttons)
{
    ClearBackground(render_opts->background_color);

    const GridDimensions* grid = &context->grid;
    const int line_thickness = 4;

    // Vertical lines
    DrawRectangle(grid->start_x + grid->cell_size - line_thickness / 2,
                  grid->start_y, line_thickness,
                  (int)grid->grid_size, BLACK);
    DrawRectangle(grid->start_x + grid->cell_size * 2 - line_thickness / 2,
                  grid->start_y, line_thickness,
                  (int)grid->grid_size, BLACK);

    // Horizontal lines
    DrawRectangle(grid->start_x,
                  grid->start_y + grid->cell_size - line_thickness / 2,
                  (int)grid->grid_size, line_thickness, BLACK);
    DrawRectangle(grid->start_x,
                  grid->start_y + grid->cell_size * 2 - line_thickness / 2,
                  (int)grid->grid_size, line_thickness, BLACK);

    // Display the current game mode
    const char* game_mode = get_game_mode_name(&context->selected_game_mode);
    const Coords text_coords = calculate_centered_text_xy(
        game_mode,
        30,
        (float)grid->start_x,
        (float)grid->start_y - 160,
        grid->grid_size,
        30
    );

    DrawText(game_mode, (int)text_coords.x, (int)text_coords.y, 30, DARKGRAY);
    const int symbol_size = grid->cell_size / 2;

    const Rectangle audio_ico_rect = calc_music_icon_rect(context, resources);

    // Music toggle icon
    const Texture2D music_icon = context->audio_disabled ? resources->music_off : resources->music_on;

    const float icon_scale = 0.08f;
    const Vector2 icon_pos = {audio_ico_rect.x, audio_ico_rect.y};

    DrawTextureEx(music_icon, icon_pos, 0.0f, icon_scale, WHITE);

    uint16_t mask = 1;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        const int row = i / BOARD_SIZE;
        const int col = i % BOARD_SIZE;
        const int draw_x = grid->start_x + col * grid->cell_size + (grid->cell_size - symbol_size) / 2;
        const int draw_y = grid->start_y + row * grid->cell_size + (grid->cell_size - symbol_size) / 2;

        if (x_board & mask)
        {
            DrawText("X", draw_x, draw_y, symbol_size, BLUE);
        }
        else if (o_board & mask)
        {
            DrawText("O", draw_x, draw_y, symbol_size, RED);
        }
        mask <<= 1;
    }

    if (show_buttons)
    {
        render_buttons(IN_GAME_BUTTONS, 1, 1, render_opts, context->memo_cache, context->needs_recalculation);
    }

    display_score(context);
    if (context->state == GAME_STATE_P1_WIN || context->state == GAME_STATE_P2_WIN)
    {
        const int winning_pattern = check_win(current_player);
        int line_start_x = 0, line_start_y = 0, line_end_x = 0, line_end_y = 0;
        const float line_width = 10;

        switch (winning_pattern)
        {
        case 0: // Top row
            line_start_x = grid->start_x;
            line_start_y = grid->start_y + grid->cell_size / 2;
            line_end_x = grid->start_x + (int)grid->grid_size;
            line_end_y = line_start_y;
            break;
        case 1: // Middle row
            line_start_x = grid->start_x;
            line_start_y = grid->start_y + grid->cell_size + grid->cell_size / 2;
            line_end_x = grid->start_x + (int)grid->grid_size;
            line_end_y = line_start_y;
            break;
        case 2: // Bottom row
            line_start_x = grid->start_x;
            line_start_y = grid->start_y + (int)grid->grid_size - grid->cell_size / 2;
            line_end_x = grid->start_x + (int)grid->grid_size;
            line_end_y = line_start_y;
            break;
        case 5: // First column
            line_start_x = grid->start_x + (int)((float)grid->cell_size / 2.6f);
            line_start_y = grid->start_y;
            line_end_x = line_start_x;
            line_end_y = grid->start_y + (int)grid->grid_size;
            break;
        case 4: // Middle column
            line_start_x = grid->start_x + grid->cell_size + (int)((float)grid->cell_size / 2.4f);
            line_start_y = grid->start_y;
            line_end_x = line_start_x;
            line_end_y = grid->start_y + (int)grid->grid_size;
            break;
        case 3: // Last column
            line_start_x = grid->start_x + (int)grid->grid_size - (int)((float)grid->cell_size / 1.6f);
            line_start_y = grid->start_y;
            line_end_x = line_start_x;
            line_end_y = grid->start_y + (int)grid->grid_size;
            break;
        case 6: // Diagonal top-left to bottom-right
            line_start_x = grid->start_x;
            line_start_y = grid->start_y;
            line_end_x = grid->start_x + (int)grid->grid_size;
            line_end_y = grid->start_y + (int)grid->grid_size;
            break;
        case 7: // Diagonal top-right to bottom left
            line_start_x = grid->start_x + (int)grid->grid_size;
            line_start_y = grid->start_y;
            line_end_x = grid->start_x;
            line_end_y = grid->start_y + (int)grid->grid_size;
            break;
        default:
            TraceLog(LOG_ERROR, "Unknown win pattern");
            break;
        }

        DrawLineEx((Vector2){(float)line_start_x, (float)line_start_y},
                   (Vector2){(float)line_end_x, (float)line_end_y},
                   line_width, RED);
    }
}

/**
 * @brief Renders main menu of the game
 * @param resources Pointer to the GameResources
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 */
void render_menu(const GameResources* resources, const UiOptions* render_opts, const GameContext* context)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    // Constants
    static const char TITLE[] = "TIC-TAC-TOE";
    static const int FONT_SIZE = 70;

    ClearBackground(render_opts->background_color);

    // Title rendering
    const Coords title_c =
        calculate_centered_text_xy(TITLE, FONT_SIZE, 0, 0, (float)screen_width, (float)FONT_SIZE);
    DrawText(TITLE, (int)title_c.x, (int)title_c.y, FONT_SIZE, DARKPURPLE);

    const float image_scale = 0.3f;
    const float scaled_width = (float)resources->main_menu_img.width * image_scale;
    const float scaled_height = (float)resources->main_menu_img.height * image_scale;
    const Vector2 image_pos = {
        ((float)screen_width - scaled_width) / 2,
        ((float)screen_height - scaled_height) / 4
    };

    DrawTextureEx(resources->main_menu_img, image_pos, 0.0f, image_scale, WHITE);

    const size_t button_count = sizeof(MAIN_MENU_BUTTONS) / sizeof(Button);

    render_buttons(MAIN_MENU_BUTTONS, button_count, 2, render_opts, context->memo_cache,
                   context->needs_recalculation);

    const Rectangle audio_ico_rect = calc_music_icon_rect(context, resources);

    // Music toggle icon
    const Texture2D music_icon = context->audio_disabled ? resources->music_off : resources->music_on;

    const float icon_scale = 0.08f;
    const Vector2 icon_pos = {audio_ico_rect.x, audio_ico_rect.y};

    DrawTextureEx(music_icon, icon_pos, 0.0f, icon_scale, WHITE);
}

/**
 * @brief Calculates dimensions and position of music toggle icon.
 * @param resources Pointer to the GameResources
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 * @details This function calculates dimensions and position of music toggle icon
 * based on window size
 *
 */
Rectangle calc_music_icon_rect(const GameContext* context, const GameResources* resources)
{
    const Texture2D music_icon = context->audio_disabled ? resources->music_off : resources->music_on;

    const float icon_scale = 0.08f;
    const Vector2 icon_pos = {(float)GetScreenWidth() - (float)music_icon.width * icon_scale - 20, 940};

    return (Rectangle){
        icon_pos.x, icon_pos.y, (float)music_icon.width * icon_scale, (float)music_icon.height * icon_scale
    };
}

/**
 * @brief Renders the game-over popup box
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 * @details
 * This function renders the game-over screen. It displays a
 * semi-transparent background, followed by a message box
 * showing the outcome of the game
 */
void render_game_over(const GameContext* context, const UiOptions* render_opts)
{
    // Constants
    static const char PLAYER1_WIN_MSG[] = "Player 1 Wins!";
    static const char HUMAN_WIN_MSG[] = "You Win!";
    static const char PLAYER2_WIN_MSG[] = "Player 2 Wins!";
    static const char DRAW_MSG[] = "It's a DRAW";
    static const char LOSE_MSG[] = "You lose!";

    const int screen_width = GetScreenWidth();

    const int screen_height = GetScreenHeight();

    // Draw a semi-transparent background
    DrawRectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 200});


    // Calculate message box dimensions
    const BoxDimensions box = calculate_centered_box_dimensions(0.5f, 0.4f, screen_height, screen_width,
                                                                context->memo_cache);

    // Draw message box
    DrawRectangle((int)box.x, (int)box.y, (int)box.width, (int)box.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box.x, box.y, box.width, box.height}, 4, RAYWHITE);


    // Determine the message based on game state
    const char* message;
    switch (context->state)
    {
    case GAME_STATE_P1_WIN:
        message = context->computer_enabled
                      ? (context->player_1 == PLAYER_X ? HUMAN_WIN_MSG : PLAYER1_WIN_MSG)
                      : PLAYER1_WIN_MSG;
        break;
    case GAME_STATE_P2_WIN:
        message = context->computer_enabled ? LOSE_MSG : PLAYER2_WIN_MSG;
        break;
    case GAME_STATE_DRAW:
        message = DRAW_MSG;
        break;
    default:
        TraceLog(LOG_WARNING, "Invalid game state in render_game_over");
        message = "";
    }

    if (is_computer_win(context))
    {
        message = LOSE_MSG;
    }

    const Coords text_c = calculate_text_xy_offset(message, 40, box.x, box.y, box.width, box.height, 0.1f, 0.5f,
                                                   context->memo_cache);


    DrawText(message, (int)text_c.x, (int)text_c.y, 40, RAYWHITE);
    const size_t button_count = sizeof(GAME_OVER_BUTTONS) / sizeof(Button);
    render_buttons(GAME_OVER_BUTTONS, button_count, 1, render_opts, context->memo_cache, context->needs_recalculation);
}

/**
 * @brief Renders the instructions page
 * @param resources Pointer to the GameResources
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 */
void render_instructions(const GameResources* resources, const UiOptions* render_opts, const GameContext* context)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    ClearBackground(render_opts->background_color);
    // Constants
    static const char TITLE[] = "INSTRUCTIONS";
    static const int FONT_SIZE = 70;
    static const char* INSTRUCTION_TEXTS[] = {
        "1. Player 1 is X and Player 2 is O.", "2. Click any grid to make your move.",
        "3. First to line three 'X's or 'O's wins the game.", "4. Click BACK to return to the main menu."
    };

    // Title rendering
    const Coords title_c =
        calculate_centered_text_xy(TITLE, FONT_SIZE, 0, 0, (float)screen_width, (float)FONT_SIZE);
    DrawText(TITLE, (int)title_c.x, (int)title_c.y, FONT_SIZE, DARKPURPLE);

    // Instructions render
    for (int i = 0; i < 4; i++)
    {
        DrawText(INSTRUCTION_TEXTS[i], 30, 120 + i * 60, 39, BLACK);
    }

    const int instructions_x = (int)((float)screen_width / 2 * 0.3f);

    // Render instruction image
    DrawTexture(resources->instructions_1, instructions_x, (int)((float)screen_height / 2 * 0.7f), WHITE);
    DrawTexture(resources->instructions_2, instructions_x, (int)((float)screen_height / 2 * 1.08f), WHITE);

    render_buttons(INSTRUCTIONS_BUTTONS, 1, 1, render_opts, context->memo_cache, context->needs_recalculation);
}

/**
 * @brief Renders the exit confirmation popup box
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 */
void render_exit(const UiOptions* render_opts, const GameContext* context)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();
    // Calculate message box dimensions
    const BoxDimensions box_dim = calculate_centered_box_dimensions(0.5f, 0.3f, screen_height, screen_width,
                                                                    context->memo_cache);
    // Draw message box
    DrawRectangle((int)box_dim.x, (int)box_dim.y, (int)box_dim.width, (int)box_dim.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_dim.x, box_dim.y, box_dim.width, box_dim.height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Do you want to exit?";
    const Coords text_cords =
        calculate_centered_text_xy(message, 30, box_dim.x, box_dim.y, box_dim.width, box_dim.height);

    DrawText(message, (int)text_cords.x, (int)text_cords.y - 115, 30, RAYWHITE);

    const size_t button_count = sizeof(EXIT_CONFIRMATION_BUTTONS) / sizeof(Button);
    render_buttons(EXIT_CONFIRMATION_BUTTONS, button_count, 1, render_opts, context->memo_cache, context->needs_recalculation);
}

/**
 * @brief Renders game mode selection popup
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 *
 * 4 buttons is displayed in the popup box with the difficulty levels
 */
void render_game_mode_choice(const UiOptions* render_opts, const GameContext* context)
{
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    // Calculate message box dimensions
    const BoxDimensions box_dim = calculate_centered_box_dimensions(0.5f, 0.4f, screen_height, screen_width,
                                                                    context->memo_cache);

    // Draw message box
    DrawRectangle((int)box_dim.x, (int)box_dim.y, (int)box_dim.width, (int)box_dim.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_dim.x, box_dim.y, box_dim.width, box_dim.height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Choose game mode!";
    const Coords text_cords =
        calculate_centered_text_xy(message, 30, box_dim.x, box_dim.y, box_dim.width, box_dim.height);

    DrawText(message, (int)text_cords.x, (int)text_cords.y - 140, 30, RAYWHITE);

    const size_t button_count = sizeof(GAME_MODE_BUTTONS) / sizeof(Button);
    render_buttons(GAME_MODE_BUTTONS, button_count, 1, render_opts, context->memo_cache, context->needs_recalculation);
}

/**
 * @brief function for handling the transition after a win/lose/draw outcome
 * @param resources Pointer to the GameResources
 * @param render_opts Pointer to the UiOptions
 * @param context Pointer to the current game context
 * Waits 1 second before the popup message box appears to allow user to see the winning grid
 */
void do_game_over_transition(const GameResources* resources, const UiOptions* render_opts, GameContext* context)
{
    if (!context->transition.active)
    {
        context->transition.start_time = GetTime();
        context->transition.active = true;
    }

    render_grid(resources, render_opts, context, false);

    const double elapsed_time = GetTime() - context->transition.start_time;

    if (elapsed_time >= 1.0)
    {
        // Semi-transparent background
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 100});

        render_game_over(context, render_opts);
    }

    display_score(context);
}
