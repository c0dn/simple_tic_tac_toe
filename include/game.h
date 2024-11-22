#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include "common.h"

// Use 16-bit string to represent a player's board
extern uint16_t x_board;
extern uint16_t o_board;

#define BOARD_SIZE 3

// macro for bit positions
#define BIT_POS(row, col) (1 << ((row) * 3 + (col)))


extern player_t current_player;


// void both_(const GameResources* res, GameContext* context);
bool is_cell_empty(int row, int col);
int check_win(player_t player);
bool check_draw(void);
player_t get_cell(int row, int col);
void set_cell(int row, int col, player_t player);
bool is_computer_win(const GameContext* context);
player_t get_human_player(const GameContext* context);
player_t get_computer_player(const GameContext* context);
void display_score(const GameContext* context);
void update_score(player_t winner, GameContext* context);
void update_game_state_and_score(GameContext* context);


#endif // GAME_H
