#include "handlers.h"

/**
 * @brief Handles mouse click events and updates game state accordingly.
 *
 * This function processes mouse clicks on the game board by converting screen
 * coordinates to board positions, validating the move, and updating the game state.
 *
 * @param mouse_pos Vector2 containing x,y coordinates of the mouse click
 * @param screen_width Width of the game window in pixels
 * @param screen_height Height of the game window in pixels
 * @param state Pointer to the current GameState to be updated based on move result
 *
 * @details
 * Process flow:
 * 1. Calculates grid dimensions and position:
 *    - Grid size is 60% of smaller screen dimension
 *    - Grid is centered on screen
 *    - Converts to cell coordinates
 * 2. Validates move:
 *    - Checks if click is within board boundaries
 *    - Verifies cell is empty using bitboard
 * 3. Updates game state:
 *    - Sets player's mark in clicked cell
 *    - Checks for draw condition
 *    - Checks for win condition
 *    - Switches current player if game continues
 *
 * The order of checks (draw before win) ensures correct game state determination.
 */
void handle_mouse_click(const Vector2 mouse_pos, const int screen_width, const int screen_height, GameState* state)
{
    const int grid_size = screen_width < screen_height ? screen_width * 0.6 : screen_height * 0.6;
    const int cell_size = grid_size / 3;
    const int start_x = (screen_width - grid_size) / 2;
    const int start_y = (screen_height - grid_size) / 2;

    // Convert mouse position to board coordinates
    const int row = (mouse_pos.y - start_y) / cell_size;
    const int col = (mouse_pos.x - start_x) / cell_size;

    // Check if click is within board and cell is empty
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && is_cell_empty(row, col))
    {
        set_cell(row, col, current_player);

        if (check_draw())
        {
            *state = GAME_STATE_DRAW;
        }
        else if (check_win(current_player))
        {
            *state = current_player == PLAYER_X ? GAME_STATE_P1_WIN : GAME_STATE_P2_WIN;
        }
        else
        {
            current_player = current_player == PLAYER_X ? PLAYER_O : PLAYER_X;
        }
    }
}
