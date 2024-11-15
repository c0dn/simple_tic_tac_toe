#ifndef COMPUTER_H
#define COMPUTER_H
#include <game.h>
#include <neural.h>


EvalResult minimax(player_t current_player);

void computer_move(const GameContext* context, const AiModels* models);
EvalResult nb_move(const BayesModel* model, player_t computer_player);
EvalResult nn_move(NeuralNetwork* nn);

#endif //COMPUTER_H
