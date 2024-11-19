//
// Created by user on 12/11/2024.
//

#ifndef NEURAL_H
#define NEURAL_H

/***************************************************************/
/*                     NEURAL NETWORK                          */
/***************************************************************/

#define INPUT_NODES 9
#define HIDDEN_NODES 12
#define OUTPUT_NODES 1

typedef struct
{
    double hidden_weights[HIDDEN_NODES][INPUT_NODES];  // Weights between input and hidden layer
    double hidden_layer[HIDDEN_NODES];                 // Outputs of hidden layer
    double output_weights[OUTPUT_NODES][HIDDEN_NODES]; // Weights between hidden and output layer
    double output_layer[OUTPUT_NODES];                 // Outputs of output layer
    double bias_hidden[HIDDEN_NODES];                  // Biases for hidden layer
    double bias_output[OUTPUT_NODES];                  // Biases for output layer

    // Adam parameters for input-to-hidden weights and biases
    double m_hidden_weights[HIDDEN_NODES][INPUT_NODES]; // First moment estimate for hidden weights
    double v_hidden_weights[HIDDEN_NODES][INPUT_NODES]; // Second moment estimate for hidden weights
    double m_bias_hidden[HIDDEN_NODES];                 // First moment estimate for hidden biases
    double v_bias_hidden[HIDDEN_NODES];                 // Second moment estimate for hidden biases

    // Adam parameters for hidden-to-output weights and biases
    double m_output_weights[OUTPUT_NODES][HIDDEN_NODES]; // First moment estimate for output weights
    double v_output_weights[OUTPUT_NODES][HIDDEN_NODES]; // Second moment estimate for output weights
    double m_bias_output[OUTPUT_NODES];                  // First moment estimate for output biases
    double v_bias_output[OUTPUT_NODES];                  // Second moment estimate for output biases
} NeuralNetwork;

typedef struct {
    int score;
    int move;
} EvalResult;


void forward_pass(NeuralNetwork *nn, const double input[]);
EvalResult nn_move(NeuralNetwork* nn);
NeuralNetwork* load_model();

#endif //NEURAL_H
