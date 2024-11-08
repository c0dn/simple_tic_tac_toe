#ifndef GAME_H
#define GAME_H

#include <menu.h>
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
    MENU_DIFF_CHOICE,
    GAME_STATE_PLAYING,
    GAME_STATE_P1_WIN,
    GAME_STATE_P2_WIN,
    GAME_STATE_DRAW,
    MENU_INSTRUCTIONS,
    MENU_SETTINGS,
    GAME_STATE_EXIT
} GameState;


typedef enum
{
    TWO_PLAYER,
    ONE_PLAYER_EASY,
    ONE_PLAYER_MEDIUM,
    ONE_PLAYER_HARD
} GameMode;


typedef struct {
    bool needs_redraw;
    GameState state;
    GameMode selected_game_mode;
    player_t player_1;
    bool computer_enabled;
} GameContext;



void initialize_game(const GameResources* res, GameContext* context);
bool is_cell_empty(int row, int col);
bool check_win(player_t player);
bool check_draw(void);
player_t get_cell(int row, int col);
void set_cell(int row, int col, player_t player);
bool is_computer_win(const GameContext* context);
player_t get_human_player(const GameContext* context);
player_t get_computer_player(const GameContext* context);


#endif // GAME_H
