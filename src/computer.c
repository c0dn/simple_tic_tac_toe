#include "computer.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <utils.h>

/**
 * @brief Load the saved neural network model from nn_weights.dat.
 * 
 * @return Pointer to the loaded NeuralNetwork struct, or NULL on failure.
 */
NeuralNetwork* load_model()
{
    const static char weights_path[] = "assets/nn_weights.dat";
    NeuralNetwork* nn = malloc(sizeof(NeuralNetwork)); // this allocates memory for the neural network
    FILE* file = fopen(weights_path, "rb"); // open the nn_weights file that is binary read mode
    if (file == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load Neural net\n");
        return NULL; // if file cannot open, return NULL
    }

    // read weights and biases from the file and load into the neural network
    fread(nn->hidden_weights, sizeof(double), HIDDEN_NODES * INPUT_NODES, file);
    fread(nn->bias_hidden, sizeof(double), HIDDEN_NODES, file);
    fread(nn->output_weights, sizeof(double), OUTPUT_NODES * HIDDEN_NODES, file);
    fread(nn->bias_output, sizeof(double), OUTPUT_NODES, file);

    fclose(file); // close the file after loading
    TraceLog(LOG_INFO, "Model loaded successfully from %s", weights_path);
    return nn; // return pointer to the loaded neural network
}


BayesModel* load_naive_bayes()
{
    const static char model_path[] = "assets/bayes_model.dat";
    BayesModel* model = malloc(sizeof(BayesModel));
    FILE* file = fopen(model_path, "rb");

    if (!file) {
        TraceLog(LOG_ERROR, "Fail to load Bayes model");
        return NULL;
    }

    fread(model->prob_x, sizeof(double), 9, file);
    fread(model->prob_o, sizeof(double), 9, file);
    fread(model->prob_b, sizeof(double), 9, file);
    fread(&model->prob_win, sizeof(double), 1, file);
    fread(&model->prob_lose, sizeof(double), 1, file);
    fread(&model->total_win, sizeof(int), 1, file);
    fread(&model->total_lose, sizeof(int), 1, file);

    fclose(file);
    TraceLog(LOG_INFO, "Model loaded from %s", model_path);

    return model;
}

/**
 * @brief Performs a forward pass through the neural network.
 * 
 * @param nn Pointer to the neural network.
 * @param input Array of input features.
 */
void forward_pass(NeuralNetwork* nn, const double input[])
{
    // compute the activations for the hidden layer
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        nn->hidden_layer[i] = nn->bias_hidden[i]; // this initializes the network with bias
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->hidden_layer[i] += nn->hidden_weights[i][j] * input[j]; // calculate weighted sum
        }
        // ReLU activation function
        nn->hidden_layer[i] = nn->hidden_layer[i] > 0 ? nn->hidden_layer[i] : 0.0;
    }

    // compute the activations for the output layer
    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        nn->output_layer[i] = nn->bias_output[i]; // this initializes the network with bias
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->output_layer[i] += nn->output_weights[i][j] * nn->hidden_layer[j]; // calculate weighted sum
        }
        // sigmoid activation function
        nn->output_layer[i] = 1.0 / (1.0 + exp(-nn->output_layer[i]));
    }
}


double predict_naive_bayes(const BayesModel* model, int const computer_player) {
    double win_probability = model->prob_win;
    double lose_probability = model->prob_lose;
    uint16_t input_x_board = 0;
    uint16_t input_o_board = 0;

    if(computer_player == PLAYER_O) {
        input_x_board = o_board;
        input_o_board = x_board;
    } else
    {
        input_x_board = x_board;
        input_o_board = o_board;
    }

    for(int pos = 0; pos < 9; pos++) {
        const uint16_t bit = 1 << pos;

        if(input_x_board & bit) {
            win_probability *= model->prob_x[pos];
            lose_probability *= model->prob_x[pos];
        }
        else if(input_o_board & bit) {
            win_probability *= model->prob_o[pos];
            lose_probability *= model->prob_o[pos];
        }
        else {
            win_probability *= model->prob_b[pos];
            lose_probability *= model->prob_b[pos];
        }
    }

    return win_probability / (win_probability + lose_probability);
}

/**
 * @brief Determines the best move for the AI using a neural network.
 * 
 * @param nn Pointer to the trained NeuralNetwork structure.
 * @param computer_player The computer's player type (PLAYER_X or PLAYER_O).
 * @return EvalResult Struct containing the best score and the index of the best move.
 */
EvalResult nn_move(NeuralNetwork* nn, const player_t computer_player)
{
    int best_move = -1;                     // initialize the index of the best move
    double best_score = -INFINITY;          // initialize the best score to negative infinity

    // Combine the X and O boards to determine occupied positions
    const uint16_t occupied = x_board | o_board;
    uint16_t legal_moves = ~occupied & 0b111111111; // Identify legal moves by masking occupied positions

    // Iterate over all legal moves
    while (legal_moves)
    {
        const int move = count_trailing_zeros(legal_moves); // Get the index of the least significant bit

        // Simulate the move for the computer player
        if (computer_player == PLAYER_X)
        {
            x_board |= 1 << move; // Simulate move as X
        }
        else
        {
            o_board |= 1 << move; // Simulate move as O
        }

        double input[INPUT_NODES];
        for (int i = 0; i < INPUT_NODES; i++)
        {
            if (computer_player == PLAYER_X)
            {
                // Encode the board state for Player X as the computer
                if (x_board & 1 << i) input[i] = 1.0;       // X occupies the position
                else if (o_board & 1 << i) input[i] = -1.0; // O occupies the position
                else input[i] = 0.0;                        // Position is empty
            }
            else
            {
                // Encode the board state for Player O as the computer
                if (o_board & 1 << i) input[i] = 1.0;       // O occupies the position
                else if (x_board & 1 << i) input[i] = -1.0; // X occupies the position
                else input[i] = 0.0;                        // Position is empty
            }
        }

        // Perform a forward pass through the neural network to evaluate the move
        forward_pass(nn, input);
        const double score = nn->output_layer[0]; // Neural network output score for the move

        // Undo the simulated move to restore the original board state
        if (computer_player == PLAYER_X)
        {
            x_board &= ~(1 << move); // Remove the move for X
        }
        else
        {
            o_board &= ~(1 << move); // Remove the move for O
        }

        // Update the best score and best move if current move is better
        if (score > best_score)
        {
            best_score = score;
            best_move = move;
        }

        // Remove the evaluated move from the legal moves
        legal_moves &= ~(1 << move);
    }

    // Return the best score and the index of the best move
    return (EvalResult){(int)best_score, best_move};
}

EvalResult nb_move(const BayesModel* model, const player_t computer_player)
{
    int best_move = -1;
    double best_score = -1;

    // Loop through all possible moves and evaluate using Naive Bayes
    const uint16_t occupied = x_board | o_board;
    uint16_t legal_moves = ~occupied & 0b111111111;

    while (legal_moves)
    {
        const int move = count_trailing_zeros(legal_moves); // Get the least significant bit

        if (computer_player == PLAYER_X)
        {
            x_board |= 1 << move;
        } else
        {
            o_board |= 1 << move;
        }



        const double score = predict_naive_bayes(model, computer_player);

        if (score > best_score)
        {
            best_score = score;
            best_move = move;
        }

        if (computer_player == PLAYER_X)
        {
            x_board &= ~(1 << move);
        } else
        {
            o_board &= ~(1 << move);
        }

        legal_moves &= ~(1 << move);
    }

    return (EvalResult){best_score, best_move};
}

/**
 * @brief Execute minimax algorithm for optimal Tic-Tac-Toe move
 *
 * Recursively evaluates board positions using minimax with alpha-beta pruning
 * to determine the best move for the current player
 *
 * @param current_player Current player (X or O)
 * @param alpha Alpha value for pruning
 * @param beta Beta value for pruning
 * @param depth Remaining search depth
 * @param context Pointer to game context
 * @return EvalResult containing best score and move
 */
// Minimax Algo
// Find the optimal move for current player
// Computer simulates opponent's move to make the optimal move
EvalResult minimax(const player_t current_player, int alpha, int beta, const int depth, const GameContext* context)
{
    // Dynamically determine the human and computer players
    const player_t human = get_human_player(context);
    const player_t computer = get_computer_player(context);
    
    // Check win conditions
    if (check_win(human) != -1) return (EvalResult){-1, -1};
    if (check_win(computer) != -1) return (EvalResult){1, -1};
    if (check_draw() || depth == 0) return (EvalResult){0, -1};

    // Ternary operator: (condition) ? (value_if_true) : (value_if_false)
    double bestScore = current_player == computer ? -2 : 2; // Initialise bestScore based on player, human is -2, computer is 2; -2 and 2 is selected -> they act as the -inf and inf, goal is to increase -2 and decrease 2
    int bestMove = -1; // Initialise bestMove to -1 because -1 is an invalid move, so new possible move detected will be updated

    // Initialized condition to check if the chosen move is legal, meaning the cell is not occupied
    const uint16_t occupied_board = x_board | o_board;
    uint16_t legal_moves = ~occupied_board & 0b111111111;

    while (legal_moves)
    {
        const int move = count_trailing_zeros(legal_moves); // Count trailing zeroes of legal_moves
        const int row = move / 3; // Determines the row number on the board by dividing move by 3
        const int col = move % 3; // Determines the column number on the board by taking the remainder when dividing the move by 3

        // Try the move
        set_cell(row, col, current_player);

        // Calls Minimax, if current player is computer, it passes X as the player in the next minimax call
        // Allows minimax to simulate moves alternately for X and O, each call represents the opponent's turn
        // Alpha: best score maximizer (computer), higher better
        // Beta: best score minimizer (human), lower better
        // Depth: decrements the depth every call, depth controls how far the algorithm explore the game tree
        const EvalResult result = minimax(
            current_player == human ? computer : human, alpha, beta, depth -1, context
        );

        // Undo move -Reset board after trying the moves
        if (current_player == PLAYER_X)
        {
            x_board &= ~BIT_POS(row, col);
        }
        else
        {
            o_board &= ~BIT_POS(row, col);
        }

        // Update best score and move
        // maximizing for computer
        if (current_player == computer) {
            if (result.score > bestScore) {
                bestScore = result.score; // bestscore is updated to the higher vakue of score (from minimax) and bestScore, aim to find the move with the highest possible score
                bestMove = move;
            }
            alpha = (alpha > bestScore) ? alpha : bestScore;// Alpha is updated to hold the maximum of alpha and bestScore, the highest value the maximizing player is assured to achieve
        } else {
            if (result.score < bestScore) {
                bestScore = result.score; // bestScore is updated to the lower value of the score (from minimax) and bestScore, aim to find the move with the lowest possible score
                bestMove = move;
            }
            beta = (beta < bestScore) ? beta : bestScore; // Beta is updated to hold the minimum of beta and bestScore, the lowest value the maximizing player is assured to achieve
        }

        // Alpha-Beta Pruning
        // Prune condition: alpha >= beta
        // Not necessary to explore current path if alpha >= beta
        // Because, for the maximizing player, a move with a score less than the alpha would be irrelevant because a better option already exists
        // Optimizes minimax as the number of possible moves being evaluated is reduced
        if (alpha >= beta) {
            break;
        }

        // Remove move from legal_moves
        legal_moves &= ~(1 << move);
    }

    return (EvalResult){bestScore, bestMove};
}


/**
 * @brief Execute computer's move using various algorithms selected by current game difficulty
 *
 * @param context Current game context
 * @param models struct containing ML model parameters
 */
void computer_move(const GameContext* context, const AiModels* models) {
    const player_t computer_player = get_computer_player(context);
    EvalResult result;

    switch (context->selected_game_mode)
    {

    case ONE_PLAYER_EASY_NAIVE:
        result = nb_move(models->bayes_model, computer_player);
        break;

    case ONE_PLAYER_EASY_NN:
        result = nn_move(models->neural_network, computer_player);
        break;

    case ONE_PLAYER_MEDIUM:
        result = minimax(computer_player, -2, 2, 3, context);
        break;

    case ONE_PLAYER_HARD:
        result = minimax(computer_player, -2, 2, 9, context);
        break;

    default:
        return;
    }

    if (result.move != -1)
    {
        const int row = result.move / 3;
        const int col = result.move % 3;
        set_cell(row, col, computer_player);
    }
}

