#include <game.h>


wchar_t board[BOARD_SIZE][BOARD_SIZE];
wchar_t current_player_symbol = L'X';



/**************************************************************/
/*  function: initialize_game                                 */
/*                                                            */
/*  Purpose:                                                  */
/*     This function initializes the game by:                 */
/*     - Resetting the game board to its default state (empty) */
/*     - Setting the player's symbol to 'X'                   */
/*     - Setting the game state to PLAYING                    */
/*  Input Parameters:                                          */
/*     GameState *state - Pointer to the game state variable   */
/*  Returns:                                                   */
/*     void                                                    */
/**************************************************************/
void initialize_game(GameState *state) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = L' ';
    *state = GAME_STATE_PLAYING;
    current_player_symbol = L'X';
}

/**************************************************************/
/*  function: is_cell_empty                                   */
/*                                                            */
/*  Purpose:                                                  */
/*     This function checks if a specific cell on the game    */
/*     board is empty.                                        */
/*                                                            */
/*  Input Parameters:                                         */
/*     int row - The row index of the cell to check.          */
/*     int col - The column index of the cell to check.       */
/*                                                            */
/*  Returns:                                                  */
/*     bool - Returns true if the cell is empty (contains a   */
/*            blank character ' '), otherwise returns false.  */
/**************************************************************/
bool is_cell_empty(const int row, const int col) {
    return board[row][col] == L' ';
}

/**************************************************************/
/*  function: check_win                                       */
/*                                                            */
/*  Purpose:                                                  */
/*     This function checks if the player with the specified  */
/*     symbol has won the game by evaluating all possible     */
/*     winning conditions on the game board.                  */
/*                                                            */
/*  Input Parameters:                                         */
/*     wchar_t player_symbol - The symbol representing the    */
/*                             current player ('X' or 'O').   */
/*                                                            */
/*  Returns:                                                  */
/*     bool - Returns true if the player has won by forming   */
/*            a line (row, column, or diagonal) with their    */
/*            symbol, otherwise returns false.                */
/**************************************************************/
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

/**************************************************************/
/*  function: check_draw                                      */
/*                                                            */
/*  Purpose:                                                  */
/*     This function checks if the game is a draw, meaning    */
/*     there are no empty spaces left on the board and        */
/*     neither player has won.                                */
/*                                                            */
/*  Input Parameters:                                         */
/*     None                                                   */
/*                                                            */
/*  Returns:                                                  */
/*     bool - Returns true if all cells on the board are      */
/*            filled (i.e., no spaces are left), indicating   */
/*            a draw. Returns false if any cell is still      */
/*            empty.                                          */
/**************************************************************/
bool check_draw(void) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (board[i][j] == L' ')
                return false;

    return true;
}