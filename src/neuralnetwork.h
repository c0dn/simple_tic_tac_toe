#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define IMPORT_FILE "C:\\Users\\limxu\\OneDrive\\Documents\\a SIT Y1.1\\Group Project\\simple_tic_tac_toe\\assets\\tic-tac-toe.data"
#define EXPORT_FILEPATH "C:\\Users\\limxu\\OneDrive\\Documents\\a SIT Y1.1\\Group Project\\simple_tic_tac_toe\\assets\\nn_weights.dat" // Path to the saved model file
#define TRAINING_SAMPLES 766
#define TESTING_SAMPLES 192
#define TOTAL_SAMPLES 958
#define RAND_STATE 11032024

// Parameters
#define INPUT_NODES 9
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.01
#define EPOCHS 2000
#define BETA1 0.9
#define BETA2 0.999
#define EPSILON 1e-8
#define LAMBDA 0.01 // Regularization strength
#define PATIENCE 15 // Number of epochs with no improvement to wait before stopping
// #define DROPOUT_RATE 0.5

// Define Sample struct
typedef struct
{
    double input[INPUT_NODES];
    double output[OUTPUT_NODES];
} Sample;

// Define NeuralNetwork struct
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

// Function prototypes
void initialize_network(NeuralNetwork *nn);
void forward_pass(NeuralNetwork *nn, double input[]);
void backward_pass(NeuralNetwork *nn, double input[], double target[], int t);
void train(NeuralNetwork *nn, double inputs[TRAINING_SAMPLES][INPUT_NODES],
                          double targets[TRAINING_SAMPLES][OUTPUT_NODES], double test_inputs[][INPUT_NODES],
                          double test_targets[][OUTPUT_NODES]);
void read_dataset(const char *filename, Sample dataset[]);
void shuffle_dataset(Sample dataset[], int n);
void split_dataset(Sample dataset[], Sample train_data[], Sample test[]);
int predict(NeuralNetwork *nn, double input[]);
void calculate_confusion_matrix(Sample *data, NeuralNetwork *nn, int size);
void print_metrics(int TP, int FP, int TN, int FN);
void print_train_and_test_scores(NeuralNetwork *nn, Sample train_data[], Sample test_data[]);
void print_weights_and_biases(NeuralNetwork *nn);
void print_board(int board[INPUT_NODES]);
bool check_winner(int board[INPUT_NODES], int player);
bool is_draw(int board[INPUT_NODES]);
void ai_move(NeuralNetwork *nn, int board[INPUT_NODES]);
void save_model(NeuralNetwork *nn, const char *nn_weights);

#endif