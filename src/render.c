#include "render.h"
#include <buttons.h>
#include "handlers.h"
#include <raylib.h>
#include <utils.h>

static void render_buttons(
    Button* buttons,
    const size_t button_count)
{
    for (int i = 0; i < button_count; i++)
    {
        Color buttonColor = buttons[i].color;
        buttons[i].rect = calculate_button_rectangle(
            buttons[i].width, buttons[i].padding, buttons[i].height, buttons[i].first_render_offset, i
        );
        // Hover effect
        if (CheckCollisionPointRec(GetMousePosition(), buttons[i].rect))
        {
            buttonColor = buttons[i].hoverColor;
        }

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
        const struct Cords cords = calculate_centered_text_xy(
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


void render_grid(const GameResources* resources)
{
    const Texture background_texture = resources->background_texture;
    DrawTexture(background_texture, 0, 0, WHITE);
    DrawRectangleLines(GetScreenWidth() / 2 - background_texture.width / 2,
                       GetScreenHeight() / 2 - background_texture.height / 2,
                       background_texture.width, background_texture.height, LIME);
    // Calculate grid size for a square centered grid
    const int grid_size = GetScreenWidth() < GetScreenHeight()
                              ? (float)GetScreenWidth() * 0.6f
                              : (float)GetScreenHeight() * 0.6f;
    const int cell_size = grid_size / 3;

    // Calculate starting position to center the grid
    const int start_x = (GetScreenWidth() - grid_size) / 2;
    const int start_y = (GetScreenHeight() - grid_size) / 2;

    // Draw thicker lines using rectangles
    const int line_thickness = 4;

    // Vertical lines
    DrawRectangle(start_x + cell_size - line_thickness / 2, start_y,
                  line_thickness, grid_size, RAYWHITE);
    DrawRectangle(start_x + cell_size * 2 - line_thickness / 2, start_y,
                  line_thickness, grid_size, RAYWHITE);

    // Horizontal lines
    DrawRectangle(start_x, start_y + cell_size - line_thickness / 2,
                  grid_size, line_thickness, RAYWHITE);
    DrawRectangle(start_x, start_y + cell_size * 2 - line_thickness / 2,
                  grid_size, line_thickness, RAYWHITE);

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
                const Color symbol_color = (cell == PLAYER_X) ? RED : DARKBLUE;

                DrawText(symbol, draw_x, draw_y, symbol_size, symbol_color);
            }
        }
    }
}


void render_menu(const GameResources* resources)
{
    // Constants
    static const char TITLE[] = "TIC-TAC-TOE";
    static const int FONT_SIZE = 70;


    // Draw background
    const Texture background_texture = resources->background_texture;
    DrawTexture(background_texture, 0, 0, WHITE);
    DrawRectangleLines(GetScreenWidth() / 2 - background_texture.width / 2,
                       GetScreenHeight() / 2 - background_texture.height / 2,
                       background_texture.width, background_texture.height, LIME);

    // Title rendering
    const struct Cords title_c = calculate_centered_text_xy(
        TITLE,
        FONT_SIZE,
        0,
        0,
        (float)GetScreenWidth(),
        (float)FONT_SIZE
    );
    DrawText(TITLE, (int)title_c.x, (int)title_c.y, FONT_SIZE, DARKPURPLE);

    const size_t button_count = sizeof(MAIN_MENU_BUTTONS) / sizeof(Button);

    render_buttons(MAIN_MENU_BUTTONS, button_count);
}


void render_game_over(const GameState* state)
{
    // Constants
    static const char* const PLAYER1_WIN_MSG = "Player 1 Wins!";
    static const char* const PLAYER2_WIN_MSG = "Player 2 Wins!";
    static const char* const DRAW_MSG = "It's a Draw!";

    // Draw a semi-transparent background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});


    // Calculate message box dimensions
    const struct BoxDimensions box = calculate_centered_box_dimensions(0.5f, 0.4f);

    // Draw message box
    DrawRectangle((int)box.x, (int)box.y, (int)box.width, (int)box.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box.x, box.y, box.width, box.height}, 4, RAYWHITE);


    // Determine the message based on game state
    const char* message;
    switch (*state)
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

    const struct Cords text_c = calculate_text_xy_offset(
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
    render_buttons(GAME_OVER_BUTTONS, button_count);
}


void render_instructions(const GameResources* resources)
{
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
    const struct Cords title_c = calculate_centered_text_xy(
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
    DrawTexture(resources->instructions_texture1,
                instructions_x, (int)((float)GetScreenHeight() / 2 * 0.7f), WHITE);
    DrawTexture(resources->instructions_texture2,
                instructions_x, (int)((float)GetScreenHeight() / 2 * 1.08f), WHITE);

    render_buttons(INSTRUCTIONS_BUTTONS, 1);
}


void render_settings(void)
{
    // Constants
    static const char TITLE[] = "SETTINGS";

    // Title rendering
    const struct Cords title_c = calculate_centered_text_xy(
        TITLE,
        70,
        0,
        0,
        (float)GetScreenWidth(),
        (float)70
    );
    DrawText(TITLE, (int)title_c.x, (int)title_c.y, 70, DARKPURPLE);

    const size_t button_count = sizeof(SETTINGS_BUTTONS) / sizeof(Button);

    render_buttons(SETTINGS_BUTTONS, button_count);
}


void render_exit(void)
{
    // Calculate message box dimensions
    const struct BoxDimensions box_dim = calculate_centered_box_dimensions(0.5f, 0.3f);
    // Draw message box
    DrawRectangle((int)box_dim.x, (int)box_dim.y, (int)box_dim.width, (int)box_dim.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_dim.x, box_dim.y, box_dim.width, box_dim.height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Do you want to exit?";
    const struct Cords text_cords = calculate_centered_text_xy(message, 30, box_dim.x, box_dim.y, box_dim.width,
                                                               box_dim.height);

    DrawText(message, (int)text_cords.x, (int)text_cords.y - 115, 30, RAYWHITE);

    const size_t button_count = sizeof(EXIT_CONFIRMATION_BUTTONS) / sizeof(Button);
    render_buttons(EXIT_CONFIRMATION_BUTTONS, button_count);
}


void render_game_mode_choice(void)
{
    // Calculate message box dimensions
    const struct BoxDimensions box_dim = calculate_centered_box_dimensions(0.5f, 0.35f);

    // Draw message box
    DrawRectangle((int)box_dim.x, (int)box_dim.y, (int)box_dim.width, (int)box_dim.height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_dim.x, box_dim.y, box_dim.width, box_dim.height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Choose game mode!";
    const struct Cords text_cords = calculate_centered_text_xy(message, 30, box_dim.x, box_dim.y, box_dim.width,
                                                               box_dim.height);

    DrawText(message, (int)text_cords.x, (int)text_cords.y - 115, 30, RAYWHITE);


    const size_t button_count = sizeof(GAME_MODE_BUTTONS) / sizeof(Button);
    render_buttons(GAME_MODE_BUTTONS, button_count);
}
