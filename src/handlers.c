#include "handlers.h"

#include <game.h>
#include <raylib.h>


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
        board[row][col] = current_player_symbol;

        // Check win condition
        if (check_win(current_player_symbol)) {
            *state = current_player_symbol == L'X' ? GAME_STATE_P1_WIN : GAME_STATE_P2_WIN;
        }
        // Check draw condition
        else if (check_draw()) {
            *state = GAME_STATE_DRAW;
        }
        // Switch player
        else {
            current_player_symbol = current_player_symbol == L'X' ? L'O' : L'X';
        }
    }
}