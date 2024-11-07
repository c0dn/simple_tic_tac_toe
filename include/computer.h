#ifndef COMPUTER_H
#define COMPUTER_H
#include <game.h>

int minimax(player_t current_player);
void computer_move(const GameContext* context);

#endif //COMPUTER_H
