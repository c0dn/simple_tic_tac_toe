#include "render.h"
#include "handlers.h"
#include <raylib.h>


static const float btn_width = 330.0f;
static const float btn_height = 100.0f;
static const float btn_roundness = 0.4f;


void render_grid(const float screen_height, const float screen_width)
{
    // Calculate grid size for a square centered grid
    const int grid_size = screen_width < screen_height ? screen_width * 0.6f : screen_height * 0.6f;
    const int cell_size = grid_size / 3;

    // Calculate starting position to center the grid
    const int start_x = ((int)screen_width - grid_size) / 2;
    const int start_y = ((int)screen_height - grid_size) / 2;

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


void render_menu(const float screen_height, const float screen_width,
                 const GameResources* resources, const GameState* state)
{
    // Constants
    static const char TITLE[] = "TIC-TAC-TOE";
    static const int FONT_SIZE = 70;
    static const int BUTTON_SPACING = 130;
    static const int BUTTON_TEXT_OFFSET = 30;
    static const float FIRST_BUTTON_OFFSET = -300.0f;

    // Title rendering
    const int title_width = MeasureText(TITLE, FONT_SIZE);
    const int title_x = ((int)screen_width - title_width) / 2;
    DrawText(TITLE, title_x, 0, FONT_SIZE, DARKPURPLE);

    Button buttons[] = {
        { // Start New Game (parent button)
            .text = "1. Start New Game",
            .color = BLUE
        },
        { // 1 Player button
            .text = "1. 1 Player",
            .color = BLUE,
            .action = start_1player
        },
        { // 2 Player button
            .text = "2. 2 Player",
            .color = BLUE,
            .action = start_2player
        },
        { // How To Play button
            .text = "2. How To Play",
            .color = BLUE,
            .action = show_instructions
        },
        { // Settings button
            .text = "3. Settings",
            .color = BLUE,
            .action = show_settings
        },
        { // Exit button
            .text = "4. Exit",
            .color = BLUE,
            .action = exit_game
        }
    };

    // Calculate button positions
    const float start_y = screen_height / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;
    buttons[0].rect = (Rectangle){
        screen_width / 2.0f - btn_width / 2.0f,
        start_y,
        btn_width,
        btn_height + 180
    };

    for (int i = 1; i < 6; i++) {
        buttons[i].rect = (Rectangle){
            screen_width / 2.0f - btn_width / 2.0f,
            start_y + (float)((i) * BUTTON_SPACING),
            btn_width,
            btn_height
        };
    }

    const Vector2 mouse_position = GetMousePosition();

    // Draw parent Start button
    DrawRectangleRounded(buttons[0].rect, btn_roundness, 20, buttons[0].color);
    DrawRectangleRoundedLines(buttons[0].rect, btn_roundness, 20, 10, GREEN);
    DrawText(buttons[0].text,
            (int)buttons[0].rect.x + 20,
            (int)buttons[0].rect.y + 25,
            34, BLACK);

    // Render and handle other buttons
    for (int i = 1; i < 6; i++) {
        if (CheckCollisionPointRec(mouse_position, buttons[i].rect)) {
            buttons[i].color = DARKBLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && buttons[i].action) {
                buttons[i].action(resources, state);
                return;
            }
        }

        DrawRectangleRounded(buttons[i].rect, btn_roundness, 20, buttons[i].color);
        DrawRectangleRoundedLines(buttons[i].rect, btn_roundness, 20, 10, GREEN);
        DrawText(buttons[i].text,
                (int)buttons[i].rect.x + 10,
                (int)buttons[i].rect.y + BUTTON_TEXT_OFFSET,
                30, BLACK);
    }
}



void render_game_over(const float screen_height, const float screen_width,
                      const GameResources* resources, GameState* state)
{
    // Constants
    static const int BUTTON_SPACING = 130;
    static const int BUTTON_TEXT_OFFSET = 30;
    static const float FIRST_BUTTON_OFFSET = -230.0f;
    static const char* const PLAYER1_WIN_MSG = "Player 1 Wins!";
    static const char* const PLAYER2_WIN_MSG = "Player 2 Wins!";
    static const char* const DRAW_MSG = "It's a Draw!";

    // Draw a semi-transparent background
    DrawRectangle(0, 0, (int)screen_width, (int)screen_height, (Color){0, 0, 0, 200});

    // Calculate message box dimensions
    const float box_width = screen_width * 0.6f;
    const float box_height = screen_height * 0.3f;
    const float box_x = (screen_width - box_width) / 2;
    const float box_y = (screen_height - box_height) / 2;

    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Determine message based on game state
    const char* message;
    switch (*state)
    {
    case GAME_STATE_P1_WIN: message = PLAYER1_WIN_MSG; break;
    case GAME_STATE_P2_WIN: message = PLAYER2_WIN_MSG; break;
    case GAME_STATE_DRAW: message = DRAW_MSG; break;
    default:
        TraceLog(LOG_WARNING, "Invalid game state in render_game_over");
        message = "";
    }

    // Center the text in the box
    const int message_font_size = 40;
    const int text_width = MeasureText(message, message_font_size);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - message_font_size) / 2;

    DrawText(message, text_x, text_y, message_font_size, RAYWHITE);

    // Button definitions
    Button buttons[] = {
        { // Restart button
            .text = "Restart Game",
            .color = YELLOW
        },
        { // Main Menu button
            .text = "Back to Main Menu",
            .color = YELLOW
        }
    };

    // Calculate button positions
    const float start_y = screen_height / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 2; i++) {
        buttons[i].rect = (Rectangle){
            screen_width / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };
    }

    const Vector2 mouse_position = GetMousePosition();

    for (int i = 0; i < 2; i++) {
        if (CheckCollisionPointRec(mouse_position, buttons[i].rect)) {
            buttons[i].color = DARKBLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(resources->fx_click);
                switch(i) {
                case 0: // Restart
                    initialize_game(state);
                    break;
                case 1: // Main Menu
                    *state = GAME_STATE_MENU;
                    break;
                }
                return;
            }
        }

        DrawRectangleRounded(buttons[i].rect, btn_roundness, 20, buttons[i].color);
        DrawRectangleRoundedLines(buttons[i].rect, btn_roundness, 20, 10, GREEN);
        DrawText(buttons[i].text,
                (int)buttons[i].rect.x + 10,
                (int)buttons[i].rect.y + BUTTON_TEXT_OFFSET,
                30, BLACK);
    }
}



void render_instructions(const float screen_height, const float screen_width, const GameResources* resources, GameState* game_state)
{
    // Constants
    static const char TITLE[] = "INSTRUCTIONS";
    static const int FONT_SIZE = 70;
    static const int BUTTON_TEXT_OFFSET = 30;
    static const float FIRST_BUTTON_OFFSET = 300.0f;
    static const char* INSTRUCTION_TEXTS[] = {
        "1. Player 1 is X and Player 2 is O.",
        "2. Click any grid to make your move.",
        "3. First to line three 'X's or 'O's wins the game.",
        "4. Click BACK to return to the main menu."
    };

    // Title rendering
    const int title_width = MeasureText(TITLE, FONT_SIZE);
    const int title_x = ((int)screen_width - title_width) / 2;
    DrawText(TITLE, title_x, 0, FONT_SIZE, DARKPURPLE);

    // Instructions render
    for (int i = 0; i < 4; i++)
    {
        DrawText(INSTRUCTION_TEXTS[i], 30, 120 + i * 60, 39, BLACK);
    }

    Button buttons[] = {
        { // Back button
            .text = "Back",
            .color = BLUE
        }
    };


    // Calculate button positions
    const float start_y = screen_height / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;
    buttons[0].rect = (Rectangle){
        screen_width / 2.0f - btn_width / 2.0f,
        start_y,
        btn_width,
        btn_height
    };


    const Vector2 mouse_position = GetMousePosition();

    for (int i = 0; i < 1; i++) {
        if (CheckCollisionPointRec(mouse_position, buttons[i].rect)) {
            buttons[i].color = DARKBLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(resources->fx_click);
                *game_state = GAME_STATE_MENU;
                return;
            }
        }

        DrawRectangleRounded(buttons[i].rect, btn_roundness, 20, buttons[i].color);
        DrawRectangleRoundedLines(buttons[i].rect, btn_roundness, 20, 10, GREEN);
        DrawText(buttons[i].text,
                (int)buttons[i].rect.x + 10,
                (int)buttons[i].rect.y + BUTTON_TEXT_OFFSET,
                30, BLACK);
    }
}


void render_settings(const float screen_height, const float screen_width,
                    const GameResources* resources, GameState* state)
{
    // Constants
    static const char TITLE[] = "SETTINGS";
    static const int FONT_SIZE = 70;
    static const int BUTTON_SPACING = 130;
    static const int BUTTON_TEXT_OFFSET = 30;
    static const float FIRST_BUTTON_OFFSET = -200.0f;

    // Title rendering
    const int title_width = MeasureText(TITLE, FONT_SIZE);
    const int title_x = ((int)screen_width - title_width) / 2;
    DrawText(TITLE, title_x, 0, FONT_SIZE, DARKPURPLE);

    Button buttons[] = {
        { // Play button
            .text = "Play",
            .color = LIGHTGRAY
        },
        { // Pause button
            .text = "Pause",
            .color = LIGHTGRAY
        },
        { // Stop button
            .text = "Stop",
            .color = LIGHTGRAY
        },
        { // Back button
            .text = "Back",
            .color = BLUE
        }
    };

    // Calculate button positions
    const float start_y = screen_height / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 4; i++) {
        buttons[i].rect = (Rectangle){
            screen_width / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };
    }

    const Vector2 mouse_position = GetMousePosition();

    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointRec(mouse_position, buttons[i].rect)) {
            buttons[i].color = DARKBLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(resources->fx_click);
                switch(i) {
                case 0:
                    if (!IsMusicStreamPlaying(resources->background_music)) {
                        PlayMusicStream(resources->background_music);
                    }
                    break;
                case 1:
                    if (IsMusicStreamPlaying(resources->background_music)) {
                        PauseMusicStream(resources->background_music);
                    }
                    break;
                case 2:
                    if (IsMusicStreamPlaying(resources->background_music)) {
                        StopMusicStream(resources->background_music);
                    }
                    break;
                case 3:
                    *state = GAME_STATE_MENU;
                    return;
                }
            }
        }

        DrawRectangleRounded(buttons[i].rect, btn_roundness, 20, buttons[i].color);
        DrawRectangleRoundedLines(buttons[i].rect, btn_roundness, 20, 10, GREEN);
        DrawText(buttons[i].text,
                (int)buttons[i].rect.x + 10,
                (int)buttons[i].rect.y + BUTTON_TEXT_OFFSET,
                30, BLACK);
    }
}



void render_exit(const float screen_height, const float screen_width,
                 const GameResources* resources, const GameState *state) {
    // Constants
    static const int FONT_SIZE = 30;
    static const int BUTTON_SPACING = 130;
    static const int BUTTON_TEXT_OFFSET = 30;
    static const float FIRST_BUTTON_OFFSET = -40.0f;

    ClearBackground(LIME);

    // Calculate message box dimensions
    const float box_width = screen_width * 0.5f;
    const float box_height = screen_height * 0.3f;
    const float box_x = (screen_width - box_width) / 2;
    const float box_y = (screen_height - box_height) / 2;

    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Prepare and draw message
    const char message[] = "Do you want to exit?";
    const int text_width = MeasureText(message, FONT_SIZE);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - FONT_SIZE) / 2;

    DrawText(message, text_x, text_y - 115, FONT_SIZE, RAYWHITE);

    // Button definations

    Button buttons[] = {
        {
            .text = "Yes",
            .color = YELLOW,
            .action = close_window
        },
        {
            .text = "No",
            .color = YELLOW,
            .action = return_to_menu
        }
    };

    // Calculate button positions
    const float start_y = screen_height / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 2; i++) {
        buttons[i].rect = (Rectangle){
            screen_width / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };
    }

    const Vector2 mouse_position = GetMousePosition();

    for (int i = 0; i < 2; i++) {
        if (CheckCollisionPointRec(mouse_position, buttons[i].rect)) {
            buttons[i].color = DARKBLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                buttons[i].action(resources, state);
                return;
            }
        }

        DrawRectangleRounded(buttons[i].rect, btn_roundness, 20, buttons[i].color);
        DrawRectangleRoundedLines(buttons[i].rect, btn_roundness, 20, 10, GREEN);
        DrawText(buttons[i].text,
                (int)buttons[i].rect.x + 10,
                (int)buttons[i].rect.y + BUTTON_TEXT_OFFSET,
                30, BLACK);
    }
}



void render_game_mode_choice(const float screen_height, const float screen_width,
                              const GameResources* resources, GameState *state) {
    // Constants
    static const int FONT_SIZE = 30;
    static const int BUTTON_SPACING = 130;
    static const int BUTTON_TEXT_OFFSET = 30;
    static const float FIRST_BUTTON_OFFSET = -90.0f;

    ClearBackground(LIME);

    // Calculate message box dimensions
    const float box_width = screen_width * 0.5f;
    const float box_height = screen_height * 0.4f;
    const float box_x = (screen_width - box_width) / 2;
    const float box_y = (screen_height - box_height) / 2;

    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Prepare and draw message
    const char message[] = "Choose game mode!";
    const int text_width = MeasureText(message, FONT_SIZE);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - FONT_SIZE) / 2;

    DrawText(message, text_x, text_y - 115, FONT_SIZE, RAYWHITE);

    // Button definitions
    Button buttons[] = {
        {
            .text = "Easy",
            .color = YELLOW,
            .action = start_easy_mode
        },
        {
            .text = "Medium",
            .color = YELLOW,
            .action = start_medium_mode
        },
        {
            .text = "Hard",
            .color = YELLOW,
            .action = start_hard_mode
        }
    };

    // Calculate button positions
    const float start_y = screen_height / 2 - btn_height / 2 + FIRST_BUTTON_OFFSET;
    for (int i = 0; i < 3; i++) {
        buttons[i].rect = (Rectangle){
            screen_width / 2.0f - btn_width / 2.0f,
            start_y + (float)(i * BUTTON_SPACING),
            btn_width,
            btn_height
        };
    }

    const Vector2 mouse_position = GetMousePosition();

    for (int i = 0; i < 3; i++) {
        if (CheckCollisionPointRec(mouse_position, buttons[i].rect)) {
            buttons[i].color = DARKBLUE;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                buttons[i].action(resources, state);
                return;
            }
        }

        DrawRectangleRounded(buttons[i].rect, btn_roundness, 20, buttons[i].color);
        DrawRectangleRoundedLines(buttons[i].rect, btn_roundness, 20, 10, GREEN);
        DrawText(buttons[i].text,
                (int)buttons[i].rect.x + 10,
                (int)buttons[i].rect.y + BUTTON_TEXT_OFFSET,
                30, BLACK);
    }
}
