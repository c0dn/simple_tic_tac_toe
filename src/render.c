#include "render.h"
#include <buttons.h>
#include "handlers.h"
#include <raylib.h>
#include <stddef.h>
#include <utils.h>

static void render_buttons(
    Button* buttons,
    const size_t button_count,
    const int buttons_per_row,
    const UiOptions* render_opts)
{
    for (int i = 0; i < button_count; i++)
    {
        const bool overwrite_default_colors = buttons[i].override_default_colors;
        const bool is_hovering = CheckCollisionPointRec(GetMousePosition(), buttons[i].rect);
        const Color buttonColor = is_hovering
                                      ? (overwrite_default_colors
                                             ? buttons[i].clickColor
                                             : render_opts->btn_clicked_color)
                                      : overwrite_default_colors ? buttons[i].color : render_opts->primary_btn_color;

        buttons[i].rect = calculate_button_rectangle(
            buttons[i].width, buttons[i].padding, buttons[i].height, buttons[i].first_render_offset, i, buttons_per_row
        );

        // Draw button
        if (buttons[i].rounded)
        {
            DrawRectangleRounded(buttons[i].rect, 0.3f, 20, buttonColor);
            DrawRectangleRoundedLines(buttons[i].rect, 0.3f, 20, 2.0f, GRAY);
        }
        else
        {
            DrawRectangleRounded(buttons[i].rect, 0, 20, buttonColor);
            DrawRectangleRoundedLines(buttons[i].rect, 0, 20, 2.0f, GRAY);
        }

        // Center text
        const Coords cords = calculate_centered_text_xy(
            buttons[i].text,
            buttons[i].font_size,
            buttons[i].rect.x,
            buttons[i].rect.y,
            buttons[i].rect.width,
            buttons[i].rect.height
        );

        DrawText(buttons[i].text, (int)cords.x, (int)cords.y, buttons[i].font_size, BLACK);
    }

}

void render_grid(const GameResources* resources, const UiOptions* render_opts, const GameContext* context)
{
    ClearBackground(render_opts->background_color);
    
    const int grid_size = GetScreenWidth() < GetScreenHeight()
                              ? (float)GetScreenWidth() * 0.6f
                              : (float)GetScreenHeight() * 0.6f;
    const int cell_size = grid_size / 3;
    
    const int start_x = (GetScreenWidth() - grid_size) / 2;
    const int start_y = (GetScreenHeight() - grid_size) / 2;
    
    const int line_thickness = 4;
    
    DrawRectangle(start_x + cell_size - line_thickness / 2, start_y, line_thickness, grid_size, BLACK);
    DrawRectangle(start_x + cell_size * 2 - line_thickness / 2, start_y, line_thickness, grid_size, BLACK);
    DrawRectangle(start_x, start_y + cell_size - line_thickness / 2, grid_size, line_thickness, BLACK);
    DrawRectangle(start_x, start_y + cell_size * 2 - line_thickness / 2, grid_size, line_thickness, BLACK);

    const int symbol_size = cell_size / 2;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            const player_t cell = get_cell(i, j);
            if (cell != PLAYER_NONE)
            {
                const char* symbol = cell == PLAYER_X ? "X" : "O";
                const int draw_x = start_x + j * cell_size + (cell_size - symbol_size) / 2;
                const int draw_y = start_y + i * cell_size + (cell_size - symbol_size) / 2;
                const Color symbol_color = (cell == PLAYER_X) ? SKYBLUE : GOLD;

                DrawText(symbol, draw_x, draw_y, symbol_size, symbol_color);
            }
        }
    }
    
    render_buttons(HOME, 1, 1, render_opts);
    display_score(context); // Call display_score with context
}


void render_menu(const GameResources* resources, const UiOptions* render_opts, const GameContext* context)
{
    // Constants
    static const char TITLE[] = "TIC-TAC-TOE";
    static const int FONT_SIZE = 70;

    ClearBackground(render_opts->background_color);

    // Title rendering
    const Coords title_c = calculate_centered_text_xy(
        TITLE,
        FONT_SIZE,
        0,
        0,
        (float)GetScreenWidth(),
        (float)FONT_SIZE
    );
    DrawText(TITLE, (int)title_c.x, (int)title_c.y, FONT_SIZE, DARKPURPLE);

    const float image_scale = 0.3f;
    const float scaled_width = (float)resources->main_menu_img.width * image_scale;
    const float scaled_height = (float)resources->main_menu_img.height * image_scale;
    const Vector2 image_pos = {
        ((float)GetScreenWidth() - scaled_width) / 2,
        ((float)GetScreenHeight() - scaled_height) / 4
    };

    DrawTextureEx(resources->main_menu_img, image_pos, 0.0f, image_scale, WHITE);

    const size_t button_count = sizeof(MAIN_MENU_BUTTONS) / sizeof(Button);

    render_buttons(MAIN_MENU_BUTTONS, button_count, 2, render_opts);

    const Rectangle audio_ico_rect = calc_music_icon_rect(context, resources);

    // Music toggle icon
    const Texture2D music_icon = context->audio_disabled
        ? resources->music_off
        : resources->music_on;

    const float icon_scale = 0.08f;
    const Vector2 icon_pos = {
        audio_ico_rect.x,
        audio_ico_rect.y
    };

    DrawTextureEx(music_icon, icon_pos, 0.0f, icon_scale, WHITE);
    reset_score();
}


Rectangle calc_music_icon_rect(const GameContext* context, const GameResources* resources)
{
    const Texture2D music_icon = context->audio_disabled
        ? resources->music_off
        : resources->music_on;

    const float icon_scale = 0.08f;
    const Vector2 icon_pos = {
        (float)GetScreenWidth() - (float)music_icon.width * icon_scale - 20,
        20
    };

    return (Rectangle){
        icon_pos.x,
        icon_pos.y,
        music_icon.width * icon_scale,
        music_icon.height * icon_scale
    };
}


void render_game_over(const GameContext* context, const UiOptions* render_opts)
{
    ClearBackground(render_opts->background_color);
    // Constants
    static const char PLAYER1_WIN_MSG[] = "Player 1 Wins!";
    static const char PLAYER2_WIN_MSG[] = "Player 2 Wins!";
    static const char DRAW_MSG[] = "A draw. How droll.";
    static const char LOSE_MSG[] = "You lose!";

    // Draw a semi-transparent background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});


    // Calculate message box dimensions
    const BoxDimensions box = calculate_centered_box_dimensions(0.5f, 0.4f);

    // Draw message box
    DrawRectangle((int)box.x, (int)box.y, (int)box.width, (int)box.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box.x, box.y, box.width, box.height}, 4, RAYWHITE);


    // Determine the message based on game state
    const char* message;
    switch (context->state)
    {
    case GAME_STATE_P1_WIN: message = PLAYER1_WIN_MSG;
        break;
    case GAME_STATE_P2_WIN: message = PLAYER2_WIN_MSG;
        break;
    case GAME_STATE_DRAW: message = DRAW_MSG;
        break;
    default:
        TraceLog(LOG_WARNING, "Invalid game state in render_game_over");
        message = "";
    }

    if (is_computer_win(context))
    {
        message = LOSE_MSG;
    }

    const Coords text_c = calculate_text_xy_offset(
        message,
        40,
        box.x,
        box.y,
        box.width,
        box.height,
        0.1f,
        0.5f
    );


    DrawText(message, (int)text_c.x, (int)text_c.y, 40, RAYWHITE);
    const size_t button_count = sizeof(GAME_OVER_BUTTONS) / sizeof(Button);
    render_buttons(GAME_OVER_BUTTONS, button_count, 1, render_opts);
}


void render_instructions(const GameResources* resources, const UiOptions* render_opts)
{
    ClearBackground(render_opts->background_color);
    // Constants
    static const char TITLE[] = "INSTRUCTIONS";
    static const int FONT_SIZE = 70;
    static const char* INSTRUCTION_TEXTS[] = {
        "1. Player 1 is X and Player 2 is O.",
        "2. Click any grid to make your move.",
        "3. First to line three 'X's or 'O's wins the game.",
        "4. Click BACK to return to the main menu."
    };

    // Title rendering
    const Coords title_c = calculate_centered_text_xy(
        TITLE,
        FONT_SIZE,
        0,
        0,
        (float)GetScreenWidth(),
        (float)FONT_SIZE
    );
    DrawText(TITLE, (int)title_c.x, (int)title_c.y, FONT_SIZE, DARKPURPLE);


    // Instructions render
    for (int i = 0; i < 4; i++)
    {
        DrawText(INSTRUCTION_TEXTS[i], 30, 120 + i * 60, 39, BLACK);
    }

    const int instructions_x = (int)((float)GetScreenWidth() / 2 * 0.3f);

    // Render instruction image
    DrawTexture(resources->instructions_1,
                instructions_x, (int)((float)GetScreenHeight() / 2 * 0.7f), WHITE);
    DrawTexture(resources->instructions_2,
                instructions_x, (int)((float)GetScreenHeight() / 2 * 1.08f), WHITE);

    render_buttons(INSTRUCTIONS_BUTTONS, 1, 1, render_opts);
}



void render_exit(const UiOptions* render_opts)
{
    // Calculate message box dimensions
    const BoxDimensions box_dim = calculate_centered_box_dimensions(0.5f, 0.3f);
    // Draw message box
    DrawRectangle((int)box_dim.x, (int)box_dim.y, (int)box_dim.width, (int)box_dim.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_dim.x, box_dim.y, box_dim.width, box_dim.height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Do you want to exit?";
    const Coords text_cords = calculate_centered_text_xy(message, 30, box_dim.x, box_dim.y, box_dim.width,
                                                         box_dim.height);

    DrawText(message, (int)text_cords.x, (int)text_cords.y - 115, 30, RAYWHITE);

    const size_t button_count = sizeof(EXIT_CONFIRMATION_BUTTONS) / sizeof(Button);
    render_buttons(EXIT_CONFIRMATION_BUTTONS, button_count, 1, render_opts);
}


void render_game_mode_choice(const UiOptions* render_opts)
{
    // Calculate message box dimensions
    const BoxDimensions box_dim = calculate_centered_box_dimensions(0.5f, 0.35f);

    // Draw message box
    DrawRectangle((int)box_dim.x, (int)box_dim.y, (int)box_dim.width, (int)box_dim.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_dim.x, box_dim.y, box_dim.width, box_dim.height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Choose game mode!";
    const Coords text_cords = calculate_centered_text_xy(message, 30, box_dim.x, box_dim.y, box_dim.width,
                                                         box_dim.height);

    DrawText(message, (int)text_cords.x, (int)text_cords.y - 115, 30, RAYWHITE);


    const size_t button_count = sizeof(GAME_MODE_BUTTONS) / sizeof(Button);
    render_buttons(GAME_MODE_BUTTONS, button_count, 1, render_opts);
}
