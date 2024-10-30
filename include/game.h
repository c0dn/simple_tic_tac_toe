#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define BOARD_SIZE 3

typedef uint8_t player_t;
#define PLAYER_NONE 0
#define PLAYER_X 1
#define PLAYER_O 2

extern player_t board[BOARD_SIZE][BOARD_SIZE];
extern player_t current_player;

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
bool check_win(player_t player);
bool check_draw(void);


#endif // GAME_H
