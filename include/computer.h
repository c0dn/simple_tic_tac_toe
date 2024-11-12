#ifndef COMPUTER_H
#define COMPUTER_H


#define EMPTY 0
#define PLAYER_x 1
#define PLAYER_o -1
#include <game.h>
#include <neural.h>


EvalResult minimax(player_t current_player);

void computer_move(const GameContext* context, const AiModels* models);

#endif //COMPUTER_H
