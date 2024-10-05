#ifndef GAME_H
#define GAME_H

#include <wchar.h>
#include <stdbool.h>

#define BOARD_SIZE 3

extern wchar_t board[BOARD_SIZE][BOARD_SIZE];

extern wchar_t current_player_symbol;

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_P1_WIN,
    GAME_STATE_P2_WIN,
    GAME_STATE_DRAW,
    GAME_STATE_EXIT
} GameState;


void initialize_game(GameState *state);

bool is_cell_empty(int row, int col);

bool check_win(wchar_t player_symbol);

bool check_draw(void);

#endif // GAME_H
