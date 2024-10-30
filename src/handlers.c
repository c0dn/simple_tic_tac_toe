#include "handlers.h"

/**
 * @brief Handles mouse click events on the game board.
 *
 * This function processes the mouse click to determine which cell on the
 * tic-tac-toe board was clicked. If the clicked cell is valid (within the
 * board boundaries and empty), it updates the board with the current player's
 * mark, checks for a win or draw condition, and updates the game state
 * accordingly.
 *
 * @param mouse_pos A Vector2 struct containing the x and y coordinates of the mouse click.
 * @param screen_width The width of the screen in pixels.
 * @param screen_height The height of the screen in pixels.
 * @param state A pointer to the current GameState which will be updated based on the game progress.
 *
 * @details
 * - The game board is assumed to be a 3x3 grid centered on the screen.
 * - `current_player` is a global variable representing the current player (either PLAYER_X or PLAYER_O).
 * - `board` is a global 3x3 array representing the game board state.
 * - `is_cell_empty(int row, int col)` is a function that checks if the specified cell is empty.
 * - `check_win(int player)` is a function that checks if the specified player has won.
 * - `check_draw()` is a function that determines if the board is full without any player winning.
 */
void handle_mouse_click(const Vector2 mouse_pos, const int screen_width, const int screen_height, GameState *state) {

    const int grid_size = screen_width < screen_height ? screen_width * 0.6 : screen_height * 0.6;
    const int cell_size = grid_size / 3;
    const int start_x = (screen_width - grid_size) / 2;
    const int start_y = (screen_height - grid_size) / 2;

    // Convert mouse position to board coordinates
    const int row = (mouse_pos.y - start_y) / cell_size;
    const int col = (mouse_pos.x - start_x) / cell_size;

    // Check if click is within board and cell is empty
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && is_cell_empty(row, col)) {
        board[row][col] = current_player;

        // Check win condition
        if (check_win(current_player)) {
            *state = current_player == PLAYER_X ? GAME_STATE_P1_WIN : GAME_STATE_P2_WIN;
        }
        // Check draw condition
        else if (check_draw()) {
            *state = GAME_STATE_DRAW;
        }
        // Switch player
        else {
            current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }
    }
}