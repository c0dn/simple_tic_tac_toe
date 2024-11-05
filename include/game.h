#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

// Use 16-bit string to represent a player's board
extern uint16_t x_board;
extern uint16_t o_board;

#define BOARD_SIZE 3

// macro for bit positions
#define BIT_POS(row, col) (1 << ((row) * 3 + (col)))

typedef uint8_t player_t;
#define PLAYER_NONE 0
#define PLAYER_X 1
#define PLAYER_O 2

extern player_t current_player;

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_DIFF_CHOICE,
    GAME_STATE_PLAYING,
    GAME_STATE_P1_WIN,
    GAME_STATE_P2_WIN,
    GAME_STATE_DRAW,
    GAME_STATE_INSTRUCTIONS,
    GAME_STATE_SETTINGS,
    GAME_STATE_EXIT
} GameState;



void initialize_game(GameState* state);
bool is_cell_empty(int row, int col);
bool check_win(player_t player);
bool check_draw(void);
player_t get_cell(int row, int col);
void set_cell(int row, int col, player_t player);


#endif // GAME_H
