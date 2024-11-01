#include "render.h"
#include <raylib.h>

/**
 * @brief Renders the tic-tac-toe grid and player symbols.
 *
 * @param screen_height Height of the game window in pixels
 * @param screen_width Width of the game window in pixels
 *
 * @details
 * Renders the game board with the following elements:
 * - A centered square grid scaled to 60% of the smaller screen dimension
 * - Grid lines drawn as white rectangles with specified thickness
 * - Player symbols (X/O) drawn in their respective positions
 * - Symbols are sized relative to cell size for consistent appearance
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

    const int symbol_size = cell_size / 2;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            const player_t cell = get_cell(i, j);
            if (cell != PLAYER_NONE) {
                const char* symbol = cell == PLAYER_X ? "X" : "O";
                const Vector2 pos = {
                    start_x + j * cell_size + (cell_size - symbol_size)/2,
                    start_y + i * cell_size + (cell_size - symbol_size)/2
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
 * @brief Renders the game over screen with the result message.
 *
 * @param screen_height Height of the game window in pixels
 * @param screen_width Width of the game window in pixels
 * @param state Current game state indicating the result (P1_WIN/P2_WIN/DRAW)
 *
 * @details
 * Displays:
 * - Semi-transparent black overlay covering the game board
 * - Centered message box (60% width, 30% height of screen)
 * - Result message ("Player 1 Wins!", "Player 2 Wins!", or "It's a Draw!")
 * - All text is automatically centered within the message box
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
