#ifndef COMPUTER_H
#define COMPUTER_H
#include <game.h>
#include <neural.h>


EvalResult minimax(player_t current_player, int alpha, int beta, int depth, const GameContext* context);

void computer_move(const GameContext* context, const AiModels* models);
EvalResult nb_move(const BayesModel* model, player_t computer_player);
EvalResult nn_move(NeuralNetwork* nn, player_t current_player);

#endif //COMPUTER_H
