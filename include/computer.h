#ifndef COMPUTER_H
#define COMPUTER_H
#include <game.h>
#include <neural.h>


EvalResult minimax(player_t current_player, int alpha, int beta, int depth, const GameContext* context);

void computer_move(const GameContext* context, const AiModels* models);

#endif //COMPUTER_H
