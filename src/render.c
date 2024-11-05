#include "render.h"
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


void render_menu(const float screen_height, const float screen_width, const GameResources* resources, GameState* state)
{
    const char text[] = "TIC-TAC-TOE";
    const int fontSize = 70;
    const int text_width = MeasureText(text, fontSize);
    const int text_x = ((int)screen_width - text_width) / 2;
    const int text_y = 0;
    // Define button rectangles size and position
    const Rectangle start_button_pos = {
        screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 - 300,
        btn_width, btn_height + 180
    };
    const Rectangle btn_1_player_pos = {
        start_button_pos.x + 10, start_button_pos.y + 70, btn_width - 20,
        btn_height - 20
    }; // Positioned inside Start
    const Rectangle btn_2_player_pos = {
        start_button_pos.x + 10, btn_1_player_pos.y + 100, btn_width - 20,
        btn_height - 20
    }; // Below 1 Player
    const Rectangle btn_instructions_pos = {
        screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 + 10,
        btn_width, btn_height
    };
    const Rectangle btn_settings_pos = {
        screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 + 140,
        btn_width, btn_height
    };
    const Rectangle btn_exit_pos = {
        screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 + 270,
        btn_width, btn_height
    };

    const Vector2 mouse_position = GetMousePosition();

    DrawRectangleRounded(start_button_pos, btn_roundness, 20, BLUE);
    DrawRectangleRoundedLines(start_button_pos, btn_roundness, 20, 10, GREEN);
    DrawText("1. Start New Game", (int)start_button_pos.x + 20, (int)start_button_pos.y + 25, 34, BLACK);

    // Check if the mouse hovers over button 1 Player, if yes change color to Darkblue, by default, it is blue
    const Color buttonColor1Player = CheckCollisionPointRec(mouse_position, btn_1_player_pos) ? DARKBLUE : BLUE;
    const Color buttonColor2Player = CheckCollisionPointRec(mouse_position, btn_2_player_pos) ? DARKBLUE : BLUE;


    // draw the "2 Player" button
    DrawRectangleRounded(btn_2_player_pos, btn_roundness, 20, buttonColor2Player);
    DrawRectangleRoundedLines(btn_2_player_pos, btn_roundness, 20, 5, GREEN);
    DrawText("2. 2 Player", (int)btn_2_player_pos.x + 10, (int)btn_2_player_pos.y + 25, 30, BLACK);

    // draw the "1 Player" button
    DrawRectangleRounded(btn_1_player_pos, btn_roundness, 20, buttonColor1Player);
    DrawRectangleRoundedLines(btn_1_player_pos, btn_roundness, 20, 5, GREEN);
    DrawText("1. 1 Player", (int)btn_1_player_pos.x + 10, (int)btn_1_player_pos.y + 25, 30, BLACK);


    // Check for button clicks for "1 Player" and "2 Player"
    if (CheckCollisionPointRec(mouse_position, btn_1_player_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_DIFF_CHOICE;
    }
    // Check for button clicks for "2 Player"
    if (CheckCollisionPointRec(mouse_position, btn_2_player_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_PLAYING;
        initialize_game(state);
    }


    // Check if mouse hovers over buttonInst, if yes change color to Darkblue, by default, it is blue
    const Color buttonColor1 = CheckCollisionPointRec(mouse_position, btn_instructions_pos) ? DARKBLUE : BLUE;
    // Check if mouse hovers over buttonSettings, if yes change color to Darkblue, by default, it is blue
    const Color buttonColor2 = CheckCollisionPointRec(mouse_position, btn_settings_pos) ? DARKBLUE : BLUE;
    // Check if mouse hovers over buttonExit, if yes change color to Darkblue, by default, it is blue
    const Color buttonColor3 = CheckCollisionPointRec(mouse_position, btn_exit_pos) ? DARKBLUE : BLUE;


    // Draw rounded rectangle for buttonInst, buttonSettings, buttonExit with rounded lines
    DrawRectangleRounded(btn_instructions_pos, btn_roundness, 20, buttonColor1);
    DrawRectangleRoundedLines(btn_instructions_pos, btn_roundness, 20, 10, GREEN);

    DrawRectangleRounded(btn_settings_pos, btn_roundness, 20, buttonColor2);
    DrawRectangleRoundedLines(btn_settings_pos, btn_roundness, 20, 10, GREEN);

    DrawRectangleRounded(btn_exit_pos, btn_roundness, 20, buttonColor3);
    DrawRectangleRoundedLines(btn_exit_pos, btn_roundness, 20, 10, GREEN);


    // Draw the text in buttonInst, buttonSettings, buttonExit
    DrawText("2. How To Play", (int)btn_instructions_pos.x + 10, (int)btn_instructions_pos.y + 30, 35, BLACK);
    DrawText("3. Settings", (int)btn_settings_pos.x + 10, (int)btn_settings_pos.y + 30, 35, BLACK);
    DrawText("4. Exit", (int)btn_exit_pos.x + 10, (int)btn_exit_pos.y + 30, 35, BLACK);


    // Check if the instruction, settings, or exit buttons are clicked and change state and page accordingly
    if (CheckCollisionPointRec(mouse_position, btn_instructions_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_INSTRUCTIONS;
    }
    if (CheckCollisionPointRec(mouse_position, btn_settings_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_SETTINGS;
    }

    if (CheckCollisionPointRec(mouse_position, btn_exit_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySound(resources->fx_click);
        *state = GAME_STATE_EXIT;
    }

    DrawText(text, text_x, text_y, fontSize, DARKPURPLE);
}


void render_game_over(const float screen_height, const float screen_width, const GameResources* resources, GameState* state)
{
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
    static const char* const PLAYER1_WIN_MSG = "Player 1 Wins!";
    static const char* const PLAYER2_WIN_MSG = "Player 2 Wins!";
    static const char* const DRAW_MSG = "It's a Draw!";


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
    const int font_size = 40;
    const int text_width = MeasureText(message, font_size);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - font_size) / 2;

    DrawText(message, text_x, text_y, font_size, RAYWHITE);


    // Define the position and size for buttonRestart and buttonMain
    const Rectangle btn_main_menu_pos = {
        (float)box_x + ((float)box_width - btn_width) / 2, (float)box_y + (float)box_height - 230,
        btn_width, btn_height
    };

    const Rectangle btn_restart_pos = {
        (float)box_x + ((float)box_width - btn_width) / 2, (float)box_y + (float)box_height - 110,
        btn_width, btn_height
    };
    const Vector2 mouse_position = GetMousePosition(); // Get mouse position

    // Set restart_color based on whether mouse hovers over buttonRestart, If hovers, change to darkblue
    const Color restart_color = CheckCollisionPointRec(mouse_position, btn_restart_pos) ? DARKBLUE : YELLOW;
    // Set restart_color based on whether mouse hovers over buttonRestart, If hovers, change to darkblue
    const Color menu_color = CheckCollisionPointRec(mouse_position, btn_main_menu_pos) ? DARKBLUE : YELLOW;

    DrawRectangleRounded(btn_restart_pos, btn_roundness, 20, restart_color);
    DrawRectangleRounded(btn_main_menu_pos, btn_roundness, 20, menu_color);

    DrawText("Back to Main Menu", (int)btn_main_menu_pos.x + 10, (int)btn_main_menu_pos.y + 30, 30, BLACK);
    DrawText("Restart Game", (int)btn_restart_pos.x + 10, (int)btn_restart_pos.y + 30, 30, BLACK);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(mouse_position, btn_restart_pos))
        {
            PlaySound(resources->fx_click);
            initialize_game(state); // Restart the game
        }
        if (CheckCollisionPointRec(mouse_position, btn_main_menu_pos))
        {
            PlaySound(resources->fx_click);
            *state = GAME_STATE_MENU;
        }
    }
}


void render_instructions(const float screen_height, const float screen_width, const GameResources* resources, GameState* game_state)
{
    const char title[] = "INSTRUCTIONS";
    const int font_size = 70;
    const char* instructions[] = {
        "1. Player 1 is X and Player 2 is O.",
        "2. Click any grid to make your move.",
        "3. First to line three 'X's or 'O's wins the game.",
        "4. Click BACK to return to the main menu."
    };

    const int title_width = MeasureText(title, font_size);
    const int title_x = ((int)screen_width - title_width) / 2;
    const int title_y = 0;
    DrawText(title, title_x, title_y, font_size, DARKPURPLE);

    for (int i = 0; i < 4; i++)
    {
        DrawText(instructions[i], 30, 120 + i * 60, 39, BLACK);
    }

    const Rectangle back_button_rect = {
        screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 + 300,
        btn_width, btn_height
    };

    const Vector2 mouse_position = GetMousePosition();

    const Color back_button_color = CheckCollisionPointRec(mouse_position, back_button_rect) ? DARKBLUE : BLUE;

    DrawRectangleRounded(back_button_rect, btn_roundness, 20, back_button_color);
    DrawRectangleRoundedLines(back_button_rect, btn_roundness, 20, 10, GREEN);
    DrawText("Back", (int)back_button_rect.x + 10, (int)back_button_rect.y + 30, 30, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_position, back_button_rect))
    {
        PlaySound(resources->fx_click);
        *game_state = GAME_STATE_MENU;
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

    // Button definitions
    typedef struct {
        Rectangle rect;
        const char* text;
        Color color;
        void (*action)(const GameResources*);
    } Button;

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


void render_exit(const float screen_height, const float screen_width, const GameResources* resources, GameState *state) {
    ClearBackground(LIME);
    // Calculate message box dimensions
    const float box_width = screen_width * 0.5f;
    const float box_height = screen_height * 0.3f;
    const float box_x = (screen_width - box_width) / 2;
    const float box_y = (screen_height - box_height) / 2;

    // Draw message box
    DrawRectangle((int)box_x, (int)box_y, (int)box_width, (int)box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Prepare and draw a message
    const char message[] = "Do you want to exit?";
    const int font_size = 30;
    const int text_width = MeasureText(message, font_size);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - font_size) / 2 ;

    DrawText(message, text_x, text_y - 115, font_size, RAYWHITE);

    // Button positions for buttonYes and buttonNo
    const Rectangle btn_yes_rec = { screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 -40  ,
                            btn_width, btn_height };
    const Rectangle btn_no_rec = { screen_width / 2 - btn_width / 2, screen_height / 2 - btn_height / 2 + 70 ,
                           btn_width, btn_height };

    const Color yesColor = CheckCollisionPointRec(GetMousePosition(), btn_yes_rec) ? DARKBLUE : YELLOW;
    const Color noColor = CheckCollisionPointRec(GetMousePosition(), btn_no_rec) ? DARKBLUE : YELLOW;

    // Draw rounded rectangle buttonYes and buttonNo with rounded lines
    DrawRectangleRounded(btn_yes_rec, btn_roundness, 20, yesColor);
    DrawRectangleRounded(btn_no_rec, btn_roundness, 20, noColor);

    // Draw text im buttonYes and buttonNo
    DrawText("Yes", (int)btn_yes_rec.x + 10, (int)btn_yes_rec.y + 30, 30, BLACK);
    DrawText("No", (int)btn_no_rec.x + 10, (int)btn_no_rec.y + 30, 30, BLACK);

    // Check if user click the buttonYes or buttonNo
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), btn_yes_rec))  // If buttonYes is clicked
        {
            PlaySound(resources->fx_click);
            CloseWindow(); // Close the window
        }
        if (CheckCollisionPointRec(GetMousePosition(), btn_no_rec)) // If buttonNo is clicked
        {
            PlaySound(resources->fx_click);
            *state = GAME_STATE_MENU; // Return to the main menu
        }
    }
}

void render_game_mode_choice(const float screen_height, const float screen_width,
                     const GameResources* resources, GameState *state) {
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
    const int font_size = 30;
    const int text_width = MeasureText(message, font_size);
    const int text_x = (int)box_x + ((int)box_width - text_width) / 2;
    const int text_y = (int)box_y + ((int)box_height - font_size) / 2;

    DrawText(message, text_x, text_y - 115, font_size, RAYWHITE);

    // Button positions for Easy, Medium, Hard
    const Rectangle btn_easy_rec = {
        screen_width / 2 - btn_width / 2,
        screen_height / 2 - btn_height / 2 - 90,
        btn_width,
        btn_height
    };
    const Rectangle btn_medium_rec = {
        screen_width / 2 - btn_width / 2,
        screen_height / 2 - btn_height / 2 + 20,
        btn_width,
        btn_height
    };
    const Rectangle btn_hard_rec = {
        screen_width / 2 - btn_width / 2,
        screen_height / 2 - btn_height / 2 + 130,
        btn_width,
        btn_height
    };

    const Vector2 mouse_pos = GetMousePosition();

    // Colors based on mouse hover
    const Color easy_color = CheckCollisionPointRec(mouse_pos, btn_easy_rec) ? DARKBLUE : YELLOW;
    const Color medium_color = CheckCollisionPointRec(mouse_pos, btn_medium_rec) ? DARKBLUE : YELLOW;
    const Color hard_color = CheckCollisionPointRec(mouse_pos, btn_hard_rec) ? DARKBLUE : YELLOW;

    // Draw rounded rectangles
    DrawRectangleRounded(btn_easy_rec, btn_roundness, 20, easy_color);
    DrawRectangleRounded(btn_medium_rec, btn_roundness, 20, medium_color);
    DrawRectangleRounded(btn_hard_rec, btn_roundness, 20, hard_color);

    // Draw button texts
    DrawText("Easy", (int)btn_easy_rec.x + 10, (int)btn_easy_rec.y + 30, 30, BLACK);
    DrawText("Medium", (int)btn_medium_rec.x + 10, (int)btn_medium_rec.y + 30, 30, BLACK);
    DrawText("Hard", (int)btn_hard_rec.x + 10, (int)btn_hard_rec.y + 30, 30, BLACK);

    // Check button clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse_pos, btn_easy_rec)) {
            PlaySound(resources->fx_click);
            initialize_game(state);
            *state = GAME_STATE_PLAYING;
        }
        else if (CheckCollisionPointRec(mouse_pos, btn_medium_rec)) {
            PlaySound(resources->fx_click);
            initialize_game(state);
            *state = GAME_STATE_PLAYING;
        }
        else if (CheckCollisionPointRec(mouse_pos, btn_hard_rec)) {
            PlaySound(resources->fx_click);
            initialize_game(state);
            *state = GAME_STATE_PLAYING;
        }
    }
}
