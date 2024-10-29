#include <game.h>


wchar_t board[BOARD_SIZE][BOARD_SIZE];
wchar_t current_player_symbol = L'X';


/**
* @brief Initializes the game state and sets up the initial conditions.
*
* This function initializes the board to be empty (all spaces are set to a space character)
* and sets the game state to playing, with 'X' as the current player symbol.
*
* @param state Pointer to the GameState structure that holds the game's current state and other necessary data.
*/
void initialize_game(GameState *state) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = L' ';
    *state = GAME_STATE_PLAYING;
    current_player_symbol = L'X';
}

/**
* @brief Checks if a cell on the game board is empty.
*
* This function checks whether the cell at the specified row and column on the game board is empty, represented by the character `L' '`.
*
* @param row The row index of the cell to check.
* @param col The column index of the cell to check.
* @return Returns true if the cell is empty (contains a space character), otherwise returns false.
*/
bool is_cell_empty(const int row, const int col) {
    return board[row][col] == L' ';
}

/**
 * @brief Check if the given player has won the game.
 *
 * This function checks all possible winning combinations on the game board
 * to determine if the specified player has won the game.
 *
 * @param player_symbol The symbol representing the player (e.g., 'X' or 'O').
 * @return true if the player has won, false otherwise.
 */
bool check_win(const wchar_t player_symbol) {
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i][0] == player_symbol &&
            board[i][1] == player_symbol &&
            board[i][2] == player_symbol)
            return true;

        if (board[0][i] == player_symbol &&
            board[1][i] == player_symbol &&
            board[2][i] == player_symbol)
            return true;
    }

    // Check diagonals
    if (board[0][0] == player_symbol &&
        board[1][1] == player_symbol &&
        board[2][2] == player_symbol)
        return true;

    if (board[0][2] == player_symbol &&
        board[1][1] == player_symbol &&
        board[2][0] == player_symbol)
        return true;

    return false;
}

/**
 * @brief Checks if the game has ended in a draw.
 *
 * This function iterates through all cells of the game board to determine
 * if there are any empty spaces left. If no empty spaces are found, the
 * game is considered a draw.
 *
 * @return true if the game has ended in a draw, false otherwise.
 */
bool check_draw(void) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (board[i][j] == L' ')
                return false;

    return true;
}