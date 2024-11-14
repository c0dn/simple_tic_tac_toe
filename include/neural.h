//
// Created by user on 12/11/2024.
//

#ifndef NEURAL_H
#define NEURAL_H

/***************************************************************/
/*                    Naive Bayers                       */
/***************************************************************/

typedef struct {
    double probX[3][3];
    double probO[3][3];
    double probBlank[3][3];
    double probWin;
    double probLose;
} BayerProbabilities;



/***************************************************************/
/*                     NEURAL NETWORK                          */
/***************************************************************/

#define INPUT_NODES 9
#define HIDDEN_NODES 8
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

typedef struct
{
    NeuralNetwork* neural_network;
    BayerProbabilities* bayer_probabilities;
} AiModels;

typedef struct {
    int score;
    int move;
} EvalResult;

BayerProbabilities* init_naive_bayes();
EvalResult nb_move(const BayerProbabilities* probs);

int predict(const char board[3], const BayerProbabilities* probs);
void forward_pass(NeuralNetwork *nn, const double input[]);
EvalResult nn_move(NeuralNetwork* nn);
NeuralNetwork* load_model();

#endif //NEURAL_H
