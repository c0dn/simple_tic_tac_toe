#include <game.h>


wchar_t board[BOARD_SIZE][BOARD_SIZE];
wchar_t current_player_symbol = L'X';



void initialize_game(GameState *state) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = L' ';
    // Set game state to playing and reset player symbol
    *state = GAME_STATE_PLAYING;
    current_player_symbol = L'X';
}

bool is_cell_empty(const int row, const int col) {
    return board[row][col] == L' ';
}


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

bool check_draw(void) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (board[i][j] == L' ')
                return false;

    return true;
}