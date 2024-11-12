#ifndef COMPUTER_H
#define COMPUTER_H

#include "C:\Users\limxu\OneDrive\Documents\a SIT Y1.1\Group Project\simple_tic_tac_toe\include\neuralnetwork.h"  // Include neural network header
#include <C:\Users\limxu\OneDrive\Documents\a SIT Y1.1\Group Project\simple_tic_tac_toe\include\game.h>

#define EMPTY 0
#define PLAYER_x 1
#define PLAYER_o -1
#define EXPORT_FILEPATH "C:\\Users\\limxu\\OneDrive\\Documents\\a SIT Y1.1\\Group Project\\simple_tic_tac_toe\\assets\\nn_weights.dat" // Path to the saved model file

typedef struct {
    int score;
    int move;
} EvalResult;

EvalResult minimax(player_t current_player);

void computer_move(const GameContext* context);

/***************************************************************/
/*                     NEURAL NETWORK                          */
/***************************************************************/

#define INPUT_NODES 9
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1

typedef struct
{
    double weights[HIDDEN_NODES][INPUT_NODES];
    double hidden_layer[HIDDEN_NODES];
    double output_weights[OUTPUT_NODES][HIDDEN_NODES];
    double output_layer[OUTPUT_NODES];
    double bias_hidden[HIDDEN_NODES];
    double bias_output[OUTPUT_NODES];

    // Adam parameters
    double m_weights[HIDDEN_NODES][INPUT_NODES];
    double v_weights[HIDDEN_NODES][INPUT_NODES];
    double m_output_weights[OUTPUT_NODES][HIDDEN_NODES];
    double v_output_weights[OUTPUT_NODES][HIDDEN_NODES];
    double m_bias_hidden[HIDDEN_NODES];
    double v_bias_hidden[HIDDEN_NODES];
    double m_bias_output[OUTPUT_NODES];
    double v_bias_output[OUTPUT_NODES];
} NeuralNetwork;

NeuralNetwork nn;

void initialize_network(NeuralNetwork *nn);
void forward_pass(NeuralNetwork *nn, double input[]);
void print_weights_and_biases(NeuralNetwork *nn);
void load_model(NeuralNetwork *nn, const char *nn_weights);
#define EXPORT_FILEPATH "C:\\Users\\limxu\\OneDrive\\Documents\\a SIT Y1.1\\Group Project\\Machine Learning (working folder)\\nn_weights.dat" // Path to the saved model file



#endif //COMPUTER_H
