//
// Created by user on 12/11/2024.
//

#ifndef NEURAL_H
#define NEURAL_H

/***************************************************************/
/*                    Naive Bayes                       */
/***************************************************************/

typedef struct {
    double prob_x[9];
    double prob_o[9];
    double prob_b[9];

    // Prior probabilities
    double prob_win;
    double prob_lose;

    // Count totals for calculating probabilities
    int total_win;
    int total_lose;
} BayesModel;



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
    BayesModel* bayes_model;
} AiModels;

typedef struct {
    int score;
    int move;
} EvalResult;

BayesModel* load_naive_bayes();
void forward_pass(NeuralNetwork *nn, const double input[]);
double predict_naive_bayes(const BayesModel* model, int computer_player);
NeuralNetwork* load_model();

#endif //NEURAL_H
