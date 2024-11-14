#include "computer.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

double probX[BOARD_SIZE][3], probO[BOARD_SIZE][3], probBlank[BOARD_SIZE][3];

double probWin, probLose;

int predict(const char board[BOARD_SIZE]) {
    double winProb = probWin, loseProb = probLose;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == 'x') {
            winProb *= probX[i][1];
            loseProb *= probX[i][0];
        } else if (board[i] == 'o') {
            winProb *= probO[i][1];
            loseProb *= probO[i][0];
        } else {
            winProb *= probBlank[i][1];
            loseProb *= probBlank[i][0];
        }
    }

    // Return the prediction (1 for win, 0 for lose)
    return winProb * 0.9 > loseProb ? 1 : 0;
}

NeuralNetwork* load_model()
{
    const static char weights_path[] = "assets/nn_weights.dat";
    NeuralNetwork* nn = malloc(sizeof(NeuralNetwork));
    FILE* file = fopen(weights_path, "rb");
    if (file == NULL)
    {
        printf("Failed to open file for loading model.\n");
        return NULL;
    }

    // Read weights and biases from the file and load them into the network
    fread(nn->weights, sizeof(double), HIDDEN_NODES * INPUT_NODES, file);
    fread(nn->bias_hidden, sizeof(double), HIDDEN_NODES, file);
    fread(nn->output_weights, sizeof(double), OUTPUT_NODES * HIDDEN_NODES, file);
    fread(nn->bias_output, sizeof(double), OUTPUT_NODES, file);

    fclose(file);
    TraceLog(LOG_INFO, "Model loaded successfully from '%s'.\n", weights_path);
    return nn;
}


void forward_pass(NeuralNetwork* nn, const double input[])
{
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        nn->hidden_layer[i] = nn->bias_hidden[i];
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->hidden_layer[i] += nn->weights[i][j] * input[j];
        }
        // ReLU activation function for the hidden layer
        nn->hidden_layer[i] = nn->hidden_layer[i] > 0 ? nn->hidden_layer[i] : 0.0;
    }

    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        nn->output_layer[i] = nn->bias_output[i];
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->output_layer[i] += nn->output_weights[i][j] * nn->hidden_layer[j];
        }
        // sigmoid activation for the output layer
        nn->output_layer[i] = 1.0 / (1.0 + exp(-nn->output_layer[i])); // Sigmoid activation
    }
}

EvalResult nn_move(NeuralNetwork* nn) {
    int best_move = -1;
    double best_score = -1;

    const uint16_t occupied = x_board | o_board;
    uint16_t legal_moves = ~occupied & 0b111111111;

    while(legal_moves) {
        const int move = __builtin_ctz(legal_moves);
        o_board |= (1 << move); // Try move

        double input[INPUT_NODES];
        for(int i = 0; i < INPUT_NODES; i++) {
            if(x_board & (1 << i)) input[i] = 1.0;
            else if(o_board & (1 << i)) input[i] = -1.0;
            else input[i] = 0.0;
        }

        forward_pass(nn, input);
        const double score = nn->output_layer[0];

        o_board &= ~(1 << move); // Undo move

        if(score > best_score) {
            best_score = score;
            best_move = move;
        }

        legal_moves &= ~(1 << move);
    }

    return (EvalResult){(int)best_score, best_move};
}

// Naive Bayes-based move selection
EvalResult nb_move() {
    int best_move = -1;
    double best_score = -1;

    // Loop through all possible moves and evaluate using Naive Bayes
    const uint16_t occupied = x_board | o_board;
    uint16_t legal_moves = ~occupied & 0b111111111;

    while (legal_moves) {
        const int move = __builtin_ctz(legal_moves); // Get the least significant bit

        o_board |= (1 << move); // Try the move for 'o'

        // Create a board state to predict
        char board[BOARD_SIZE] = {0};
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (x_board & (1 << i)) board[i] = 'x';
            else if (o_board & (1 << i)) board[i] = 'o';
            else board[i] = 'b';
        }

        // Use Naive Bayes to predict the outcome for this move
        const int predicted_outcome = predict(board);

        // We are aiming for a 'win' (1) for the computer, but minimizing the loss
        const double score = predicted_outcome == 1 ? 1.0 : 0.0;

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }

        o_board &= ~(1 << move); // Undo the move

        legal_moves &= ~(1 << move); // Remove the move from legal moves
    }

    return (EvalResult){best_score, best_move};
}

EvalResult minimax(const player_t current_player)
{
    // Check win conditions
    if (check_win(PLAYER_X)) return (EvalResult){-1, -1};
    if (check_win(PLAYER_O)) return (EvalResult){1, -1};
    if (check_draw()) return (EvalResult){0, -1};

    int bestScore = (current_player == PLAYER_O) ? -2 : 2;
    int bestMove = -1;

    const uint16_t occupied_board = x_board | o_board;
    uint16_t legal_moves = ~occupied_board & 0b111111111;

    while (legal_moves)
    {
        const int move = __builtin_ctz(legal_moves);
        const int row = move / 3;
        const int col = move % 3;

        set_cell(row, col, current_player);

        const EvalResult result = minimax(
            current_player == PLAYER_X ? PLAYER_O : PLAYER_X
        );

        // Undo move
        if (current_player == PLAYER_X)
        {
            x_board &= ~BIT_POS(row, col);
        }
        else
        {
            o_board &= ~BIT_POS(row, col);
        }

        // Update best score and move
        if ((current_player == PLAYER_O && result.score > bestScore) ||
            (current_player == PLAYER_X && result.score < bestScore))
        {
            bestScore = result.score;
            bestMove = move;
        }

        legal_moves &= ~(1 << move);
    }

    return (EvalResult){bestScore, bestMove};
}


// Computer's move function (adjusted to include Naive Bayes)
void computer_move(const GameContext* context, const AiModels* models) {
    const player_t computer_player = get_computer_player(context);
    EvalResult result;

    switch (context->selected_game_mode) {
    case ONE_PLAYER_EASY:
        result = nn_move(models->neural_network);
        break;

    case ONE_PLAYER_EASY_NAIVE:
        result = nb_move();
        break;

    case ONE_PLAYER_MEDIUM:
        // You can add another AI logic here for medium difficulty
        break;

    case ONE_PLAYER_HARD:
        result = minimax(computer_player);
        break;

    default:
        return;
    }

    // Execute the best move if available
    if (result.move != -1) {
        const int row = result.move / 3;
        const int col = result.move % 3;
        set_cell(row, col, computer_player);
    }
}

// Function to initialize Naive Bayes probabilities (this should be called during setup)
void init_naive_bayes() {
    // Example initialization of probabilities (you should adjust these based on your dataset)
    probWin = 0.6;
    probLose = 0.4;

    for (int i = 0; i < BOARD_SIZE; i++) {
        probX[i][1] = 0.6; // Probability of 'x' in a winning state
        probO[i][1] = 0.5; // Probability of 'o' in a winning state
        probBlank[i][1] = 0.8; // Probability of a blank space in a winning state

        probX[i][0] = 0.4; // Probability of 'x' in a losing state
        probO[i][0] = 0.3; // Probability of 'o' in a losing state
        probBlank[i][0] = 0.2; // Probability of a blank space in a losing state
    }
}

