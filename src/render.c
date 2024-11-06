#include "render.h"
#include "handlers.h"
#include <raylib.h>




void render_grid(const GameResources* resources)
{
    const Texture background_texture = resources->background_texture;
    DrawTexture(background_texture, 0, 0, WHITE);
    DrawRectangleLines(GetScreenWidth() / 2 - background_texture.width / 2,
                       GetScreenHeight() / 2 - background_texture.height / 2,
                       background_texture.width, background_texture.height, LIME);
    // Calculate grid size for a square centered grid
    const int grid_size = GetScreenWidth() < GetScreenHeight() ? (float)GetScreenWidth() * 0.6f : (float)GetScreenHeight() * 0.6f;
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
    static const int BUTTON_SPACING = 130;
    static const float FIRST_BUTTON_OFFSET = -300.0f;

    static const float BTN_WIDTH = 330.0f;
    static const float BTN_HEIGHT = 100.0f;
    static const float BTN_ROUNDNESS = 0.4f;

    // Draw background
    const Texture background_texture = resources->background_texture;
    DrawTexture(background_texture, 0, 0, WHITE);
    DrawRectangleLines(GetScreenWidth() / 2 - background_texture.width / 2,
                       GetScreenHeight() / 2 - background_texture.height / 2,
                       background_texture.width, background_texture.height, LIME);

    // Title rendering
    const int title_width = MeasureText(TITLE, FONT_SIZE);
    const int title_x = (GetScreenWidth() - title_width) / 2;
    DrawText(TITLE, title_x, 0, FONT_SIZE, DARKPURPLE);

    const Button buttons[] = {
        {.text = "1 Player", .color = BLUE},
        {.text = "2 Player", .color = BLUE},
        {.text = "How To Play", .color = BLUE},
        {.text = "Settings", .color = BLUE},
        {.text = "Exit", .color = BLUE}
    };

    // Calculate button positions
    const float start_y = (float)GetScreenHeight() / 2 - BTN_HEIGHT / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 5; i++)
    {
        const Rectangle btnRect = {
            (float)GetScreenWidth() / 2.0f - BTN_WIDTH / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            BTN_WIDTH,
            BTN_HEIGHT
        };

        Color buttonColor = buttons[i].color;
        if (CheckCollisionPointRec(GetMousePosition(), btnRect))
        {
            buttonColor = DARKBLUE;
        }

        DrawRectangleRounded(btnRect, BTN_ROUNDNESS, 20, buttonColor);
        DrawRectangleRoundedLines(btnRect, BTN_ROUNDNESS, 20, 2, GRAY);

        // Center text
        const int text_width = MeasureText(buttons[i].text, 30);
        const int text_x = (int)(btnRect.x + (btnRect.width - (float)text_width) / 2);
        const int text_y = (int)(btnRect.y + (btnRect.height - 30) / 2);
        DrawText(buttons[i].text, text_x, text_y, 30, BLACK);
    }
}


void render_game_over(const GameState* state)
{
    // Constants
    static const float BUTTON_SPACING = 50.0f;
    static const char* const PLAYER1_WIN_MSG = "Player 1 Wins!";
    static const char* const PLAYER2_WIN_MSG = "Player 2 Wins!";
    static const char* const DRAW_MSG = "It's a Draw!";
    static const float BTN_WIDTH = 200.0f;
    static const float BTN_HEIGHT = 60.0f;
    static const float BTN_ROUNDNESS = 0.3f;

    // Draw a semi-transparent background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});


    // Calculate message box dimensions
    const float box_width = (float)GetScreenWidth() * 0.5f;
    const float box_height = (float)GetScreenHeight() * 0.4f;
    const float box_x = ((float)GetScreenWidth() - box_width) / 2;
    const float box_y = ((float)GetScreenHeight() - box_height) / 2;

    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

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

    // Center the text in the box
    const int message_font_size = 40;
    const int text_width = MeasureText(message, message_font_size);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + 40;

    DrawText(message, text_x, text_y, message_font_size, RAYWHITE);

    // Buttons
    Button buttons[] = {
        {.text = "Restart Game", .color = YELLOW},
        {.text = "Back to Menu", .color = YELLOW}
    };

    const float total_buttons_width = (BTN_WIDTH * 2) + BUTTON_SPACING;
    const float start_x = box_x + (box_width - total_buttons_width) / 2;
    const float buttons_y = box_y + box_height - BTN_HEIGHT - 40;

    for (int i = 0; i < 2; i++) {
        buttons[i].rect = (Rectangle){
            start_x + (BTN_WIDTH + BUTTON_SPACING) * i,
            buttons_y,
            BTN_WIDTH,
            BTN_HEIGHT
        };

        Color buttonColor = buttons[i].color;
        if (CheckCollisionPointRec(GetMousePosition(), buttons[i].rect)) {
            buttonColor = DARKBLUE;
        }

        DrawRectangleRounded(buttons[i].rect, BTN_ROUNDNESS, 20, buttonColor);
        DrawRectangleRoundedLines(buttons[i].rect, BTN_ROUNDNESS, 20, 2.0f, GRAY);

        // Center text in button
        const int btn_text_width = MeasureText(buttons[i].text, 20);
        const int btn_text_x = (int)(buttons[i].rect.x + (buttons[i].rect.width - (float)btn_text_width) / 2);
        const int btn_text_y = (int)(buttons[i].rect.y + (buttons[i].rect.height - 30) / 2);
        DrawText(buttons[i].text, btn_text_x, btn_text_y, 20, BLACK);
    }
}


void render_instructions(const GameResources* resources)
{
    // Constants
    static const float BTN_WIDTH = 330.0f;
    static const float BTN_HEIGHT = 100.0f;
    static const float BTN_ROUNDNESS = 0.4f;
    static const char TITLE[] = "INSTRUCTIONS";
    static const int FONT_SIZE = 70;
    static const float FIRST_BUTTON_OFFSET = 300.0f;
    static const char* INSTRUCTION_TEXTS[] = {
        "1. Player 1 is X and Player 2 is O.",
        "2. Click any grid to make your move.",
        "3. First to line three 'X's or 'O's wins the game.",
        "4. Click BACK to return to the main menu."
    };

    // Title rendering
    const int title_width = MeasureText(TITLE, FONT_SIZE);
    const int title_x = (GetScreenWidth() - title_width) / 2;
    DrawText(TITLE, title_x, 0, FONT_SIZE, DARKPURPLE);

    // Instructions render
    for (int i = 0; i < 4; i++)
    {
        DrawText(INSTRUCTION_TEXTS[i], 30, 120 + i * 60, 39, BLACK);
    }

    Button buttons[] = {
        {.text = "Back", .color = BLUE}
    };


    // Calculate button positions
    const float start_y = (float)GetScreenHeight() / 2 - BTN_HEIGHT / 2 + FIRST_BUTTON_OFFSET;
    buttons[0].rect = (Rectangle){
        (float)GetScreenWidth() / 2.0f - BTN_WIDTH / 2.0f,
        start_y,
        BTN_WIDTH,
        BTN_HEIGHT
    };

    const int instructions_x = (int)((float)GetScreenWidth() / 2 * 0.3f);

    // Render instruction image
    DrawTexture(resources->instructions_texture1,
                instructions_x, (int)((float)GetScreenHeight() / 2 * 0.7f), WHITE);
    DrawTexture(resources->instructions_texture2,
                instructions_x, (int)((float)GetScreenHeight() / 2 * 1.08f), WHITE);

    Color buttonColor = buttons[0].color;
    if (CheckCollisionPointRec(GetMousePosition(), buttons[0].rect))
    {
        buttonColor = DARKBLUE;
    }

    DrawRectangleRounded(buttons[0].rect, BTN_ROUNDNESS, 20, buttonColor);
    DrawRectangleRoundedLines(buttons[0].rect, BTN_ROUNDNESS, 20, 2.0f, GRAY);

    // Center text in button
    const int btn_text_width = MeasureText(buttons[0].text, 30);
    const int btn_text_x = (int)(buttons[0].rect.x + (buttons[0].rect.width - (float)btn_text_width) / 2);
    const int btn_text_y = (int)(buttons[0].rect.y + (buttons[0].rect.height - 30) / 2);
    DrawText(buttons[0].text, btn_text_x, btn_text_y, 30, BLACK);
}


void render_settings(void)
{
    // Constants
    static const float BTN_WIDTH = 330.0f;
    static const float BTN_HEIGHT = 100.0f;
    static const float BTN_ROUNDNESS = 0.4f;
    static const char TITLE[] = "SETTINGS";
    static const int FONT_SIZE = 70;
    static const int BUTTON_SPACING = 130;
    static const float FIRST_BUTTON_OFFSET = -200.0f;

    // Title rendering
    const int title_width = MeasureText(TITLE, FONT_SIZE);
    const int title_x = (GetScreenWidth() - title_width) / 2;
    DrawText(TITLE, title_x, 0, FONT_SIZE, DARKPURPLE);

    Button buttons[] = {
        {.text = "Play", .color = LIGHTGRAY},
        {.text = "Pause", .color = LIGHTGRAY},
        {.text = "Stop", .color = LIGHTGRAY},
        {.text = "Back", .color = BLUE}
    };

    // Calculate button positions
    const float start_y = (float)GetScreenHeight() / 2 - BTN_HEIGHT / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 4; i++)
    {
        buttons[i].rect = (Rectangle){
            (float)GetScreenWidth() / 2.0f - BTN_WIDTH / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            BTN_WIDTH,
            BTN_HEIGHT
        };
    }

    for (int i = 0; i < 4; i++)
    {
        Color buttonColor = buttons[i].color;
        if (CheckCollisionPointRec(GetMousePosition(), buttons[i].rect))
        {
            buttonColor = DARKBLUE;
        }

        DrawRectangleRounded(buttons[i].rect, BTN_ROUNDNESS, 20, buttonColor);
        DrawRectangleRoundedLines(buttons[i].rect, BTN_ROUNDNESS, 20, 2, GRAY);

        // Center text in button
        const int text_width = MeasureText(buttons[i].text, 30);
        const int text_x = (int)(buttons[i].rect.x + (buttons[i].rect.width - (float)text_width) / 2);
        const int text_y = (int)(buttons[i].rect.y + (buttons[i].rect.height - 30) / 2);

        DrawText(buttons[i].text, text_x, text_y, 30, BLACK);
    }
}


void render_exit(void)
{
    // Constants
    static const float BTN_WIDTH = 330.0f;
    static const float BTN_HEIGHT = 100.0f;
    static const float BTN_ROUNDNESS = 0.4f;
    static const int FONT_SIZE = 30;
    static const int BUTTON_SPACING = 130;
    static const float FIRST_BUTTON_OFFSET = -40.0f;

    // Calculate message box dimensions
    const float box_width = (float)GetScreenWidth() * 0.5f;
    const float box_height = (float)GetScreenHeight() * 0.3f;
    const float box_x = ((float)GetScreenWidth() - box_width) / 2;
    const float box_y = ((float)GetScreenHeight() - box_height) / 2;

    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Do you want to exit?";
    const int text_width = MeasureText(message, FONT_SIZE);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - FONT_SIZE) / 2;

    DrawText(message, text_x, text_y - 115, FONT_SIZE, RAYWHITE);

    // Button definitions

    Button buttons[] = {
        {.text = "Yes", .color = YELLOW},
        {.text = "No", .color = YELLOW}
    };

    // Calculate button positions
    const float start_y = (float)GetScreenHeight() / 2 - BTN_HEIGHT / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 2; i++)
    {
        buttons[i].rect = (Rectangle){
            (float)GetScreenWidth() / 2.0f - BTN_WIDTH / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            BTN_WIDTH,
            BTN_HEIGHT
        };

        // Draw button
        Color buttonColor = buttons[i].color;
        if (CheckCollisionPointRec(GetMousePosition(), buttons[i].rect))
        {
            buttonColor = DARKBLUE;
        }

        DrawRectangleRounded(buttons[i].rect, BTN_ROUNDNESS, 20, buttonColor);
        DrawRectangleRoundedLines(buttons[i].rect, BTN_ROUNDNESS, 20, 2.0f, GRAY);

        // Center text in button
        const int btn_text_width = MeasureText(buttons[i].text, 30);
        const int btn_text_x = (int)(buttons[i].rect.x + (buttons[i].rect.width - (float)btn_text_width) / 2);
        const int btn_text_y = (int)(buttons[i].rect.y + (buttons[i].rect.height - 30) / 2);
        DrawText(buttons[i].text, btn_text_x, btn_text_y, 30, BLACK);
    }
}


void render_game_mode_choice(void)
{
    // Constants
    static const int FONT_SIZE = 30;
    static const int BUTTON_SPACING = 130;
    static const float FIRST_BUTTON_OFFSET = -90.0f;
    static const float BTN_WIDTH = 330.0f;
    static const float BTN_HEIGHT = 100.0f;
    static const float BTN_ROUNDNESS = 0.4f;

    // Calculate message box dimensions
    const float box_width = (float)GetScreenWidth() * 0.6f;
    const float box_height = (float)GetScreenHeight() * 0.4f;
    const float box_x = ((float)GetScreenWidth() - box_width) / 2;
    const float box_y = ((float)GetScreenHeight() - box_height) / 2;


    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Draw message
    const char message[] = "Choose game mode!";
    const int text_width = MeasureText(message, FONT_SIZE);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - FONT_SIZE) / 2;

    DrawText(message, text_x, text_y - 115, FONT_SIZE, RAYWHITE);

    // Button definitions
    Button buttons[] = {
        {.text = "Easy", .color = YELLOW},
        {.text = "Medium", .color = YELLOW},
        {.text = "Hard", .color = YELLOW}
    };

    // Calculate button positions
    const float start_y = (float)GetScreenHeight() / 2 - BTN_HEIGHT / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 3; i++)
    {
        buttons[i].rect = (Rectangle){
            (float)GetScreenWidth() / 2.0f - BTN_WIDTH / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            BTN_WIDTH,
            BTN_HEIGHT
        };

        // Draw button
        Color buttonColor = buttons[i].color;
        if (CheckCollisionPointRec(GetMousePosition(), buttons[i].rect))
        {
            buttonColor = DARKBLUE;
        }

        DrawRectangleRounded(buttons[i].rect, BTN_ROUNDNESS, 20, buttonColor);
        DrawRectangleRoundedLines(buttons[i].rect, BTN_ROUNDNESS, 20, 2.0f, GRAY);

        // Center text in button
        const int btn_text_width = MeasureText(buttons[i].text, 30);
        const int btn_text_x = (int)(buttons[i].rect.x + (buttons[i].rect.width - (float)btn_text_width) / 2);
        const int btn_text_y = (int)(buttons[i].rect.y + (buttons[i].rect.height - 30) / 2);
        DrawText(buttons[i].text, btn_text_x, btn_text_y, 30, BLACK);
    }
}
