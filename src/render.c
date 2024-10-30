#include "render.h"
#include <raylib.h>

/**
 * @brief Renders the tic-tac-toe grid and player symbols on the screen.
 *
 * This function draws a 3x3 tic-tac-toe grid centered on the screen and
 * displays the player symbols (X and O) in their respective cells based on
 * the current game board state.
 *
 * @param screen_height The height of the screen in pixels.
 * @param screen_width The width of the screen in pixels.
 *
 * @details
 * - The grid is drawn as a square, with each cell occupying one-third of the grid size.
 * - The grid is centrally positioned on the screen, with line thickness used to draw grid lines.
 * - Vertical and horizontal lines are drawn using rectangles for better visual thickness and clarity.
 */
void render_grid(const int screen_height, const int screen_width) {
    // Calculate grid size for a square centered grid
    const int grid_size = screen_width < screen_height ? screen_width * 0.6 : screen_height * 0.6;
    const int cell_size = grid_size / 3;

    // Calculate starting position to center the grid
    const int start_x = (screen_width - grid_size) / 2;
    const int start_y = (screen_height - grid_size) / 2;

    // Draw thicker lines using rectangles
    const int line_thickness = 4;

    // Vertical lines
    DrawRectangle(start_x + cell_size - line_thickness/2, start_y,
                  line_thickness, grid_size, RAYWHITE);
    DrawRectangle(start_x + cell_size * 2 - line_thickness/2, start_y,
                  line_thickness, grid_size, RAYWHITE);

    // Horizontal lines
    DrawRectangle(start_x, start_y + cell_size - line_thickness/2,
                  grid_size, line_thickness, RAYWHITE);
    DrawRectangle(start_x, start_y + cell_size * 2 - line_thickness/2,
                  grid_size, line_thickness, RAYWHITE);

    const int symbol_size = cell_size / 2;  // Size of X and O
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != PLAYER_NONE) {
                const char* symbol = (board[i][j] == PLAYER_X) ? "X" : "O";
                const Vector2 pos = {
                    start_x + (j * cell_size) + (cell_size - symbol_size)/2,
                    start_y + (i * cell_size) + (cell_size - symbol_size)/2
                };
                DrawText(symbol, pos.x, pos.y, symbol_size, RAYWHITE);
            }
        }
    }
}


void render_menu(const int screen_height, const int screen_width) {
    DrawRectangle(0, 0, screen_width, screen_height, GREEN);
    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
    DrawText("PRESS ENTER to jump to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

/**
 * @brief Renders the game over screen based on the game's final state.
 *
 * This function displays a semi-transparent overlay and a message box
 * indicating the result of the tic-tac-toe game such as Player 1 winning,
 * Player 2 winning, or a draw.
 * It also prompts the user to press ENTER
 * to restart the game.
 *
 * @param screen_height The height of the screen in pixels.
 * @param screen_width The width of the screen in pixels.
 * @param state The final GameState that indicates the outcome (e.g., win or draw).
 *
 * @details
 * - The message box is drawn in the center of the screen with a dark gray
 *   background and white borderlines.
 *   The box occupies 60% of the screen's width and 30% of its height.
 * - Text including the game outcome and a prompt to play again are centered within the message box.
 */
void render_game_over(const int screen_height, const int screen_width, const GameState state) {
    // Draw a semi-transparent background
    DrawRectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 200});

    // Calculate message box dimensions
    const int box_width = screen_width * 0.6;
    const int box_height = screen_height * 0.3;
    const int box_x = (screen_width - box_width) / 2;
    const int box_y = (screen_height - box_height) / 2;

    // Draw message box
    DrawRectangle(box_x, box_y, box_width, box_height, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){box_x, box_y, box_width, box_height}, 4, RAYWHITE);

    // Prepare and draw message
    const char* message;
    switch(state) {
    case GAME_STATE_P1_WIN:
        message = "Player 1 Wins!";
        break;
    case GAME_STATE_P2_WIN:
        message = "Player 2 Wins!";
        break;
    case GAME_STATE_DRAW:
        message = "It's a Draw!";
        break;
    default:
        message = "";
    }

    // Center the text in the box
    const int font_size = 40;
    const int text_width = MeasureText(message, font_size);
    const int text_x = box_x + (box_width - text_width) / 2;
    const int text_y = box_y + (box_height - font_size) / 2;

    DrawText(message, text_x, text_y, font_size, RAYWHITE);
    DrawText("Press ENTER to play again",
             box_x + (box_width - MeasureText("Press ENTER to play again", 20)) / 2,
             text_y + font_size + 10,
             20, RAYWHITE);
}
