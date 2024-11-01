#include <game.h>

player_t board[BOARD_SIZE][BOARD_SIZE];
player_t current_player;

/**
 * @brief Initializes the game by setting up the board and initial game state.
 *
 * This function is responsible for preparing the tic-tac-toe game for a new play session.
 * It clears the board, sets the initial game state, and designates the starting player.
 *
 * @param state A pointer to a GameState variable that will be set to GAME_STATE_PLAYING.
 *
 * @details
 * The board is cleared by setting all cells to PLAYER_NONE, indicating they are unoccupied.
 * - The game state is set to GAME_STATE_PLAYING, indicating an active game session.
 * - current_player is initialized to PLAYER_X, indicating that Player X will make the first move.
 * - The board is defined as a global 3x3 grid.
 */
void initialize_game(GameState *state) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = PLAYER_NONE;
    *state = GAME_STATE_PLAYING;
    current_player = PLAYER_X;
}

/**
 * @brief Checks if a specific cell on the board is empty.
 *
 * This function evaluates the specified cell's status on the tic-tac-toe board
 * to determine if it is unoccupied.
 *
 * @param row The row index of the cell to check.
 * @param col The column index of the cell to check.
 * @return Returns true if the cell is empty (PLAYER_NONE), false if it is occupied.
 *
 * @details
 * The board is assumed to be a 3x3 grid, and row and column indices should be within valid bounds.
 * PLAYER_NONE is a constant indicating that a cell is unoccupied.
 */
bool is_cell_empty(const int row, const int col) {
    return board[row][col] == PLAYER_NONE;
}

/**
 * @brief Checks if the given player has won the game.
 *
 * This function examines the current state of the tic-tac-toe board to determine
 * if the specified player has achieved a winning configuration. A win is defined
 * as having all three cells in any row, column, or diagonal occupied by the player's mark.
 *
 * @param player The player to check for a win condition (PLAYER_X or PLAYER_O).
 * @return Returns true if the player has won, false otherwise.
 *
 * @details
 * The function iterates over each row and column to check for a sequence of three matching player marks.
 * - It also checks both main diagonals for a sequence of three matching player marks.
 * - The board is defined as a 3x3 grid.
 */
bool check_win(const player_t player) {
    // Check rows
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i][0] == player) {  // Only check further if the first cell matches
            if (board[i][1] == player && board[i][2] == player)
                return true;
        }
    }

    // Check columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[0][i] == player) {  // Only check further if the first cell matches
            if (board[1][i] == player && board[2][i] == player)
                return true;
        }
    }

    // Check diagonals - only if the center is matched
    if (board[1][1] == player) {
        if ((board[0][0] == player && board[2][2] == player) ||
            (board[0][2] == player && board[2][0] == player))
            return true;
    }

    return false;
}


/**
 * @brief Checks if the game board is in a draw state.
 *
 * This function iterates over the entire tic-tac-toe board to determine
 * if all cells are occupied, and there is no winner, indicating a draw.
 *
 * @return Returns true if all cells on the board are filled and none are empty,
 *         which means the game is a draw. Returns false if at least one cell is
 *         still unoccupied (indicated by PLAYER_NONE).
 *
 * @details
 * BOARD_SIZE is defined as the dimension of the board (typically 3 for a 3x3 grid).
 * - board is a global 2D array representing the state of the game.
 * - PLAYER_NONE is a constant indicating an empty cell.
 * - This function should be called after a move to check if the game has ended in a draw.
 */
bool check_draw(void) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (board[i][j] == PLAYER_NONE)
                return false;

    return true;
}