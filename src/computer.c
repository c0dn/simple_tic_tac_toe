#include "computer.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

NeuralNetwork* load_model()
{
    const static char weights_path[] = "assets/nn_weights.dat";
    NeuralNetwork* nn = malloc(sizeof(NeuralNetwork));
    FILE* file = fopen(weights_path, "rb");
    if (file == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load Neural net\n");
        return NULL;
    }

    // Read weights and biases from the file and load them into the network
    fread(nn->hidden_weights, sizeof(double), HIDDEN_NODES * INPUT_NODES, file);
    fread(nn->bias_hidden, sizeof(double), HIDDEN_NODES, file);
    fread(nn->output_weights, sizeof(double), OUTPUT_NODES * HIDDEN_NODES, file);
    fread(nn->bias_output, sizeof(double), OUTPUT_NODES, file);

    fclose(file);
    TraceLog(LOG_INFO, "Model loaded successfully from '%s'.\n", weights_path);
    return nn;
}

BayesModel* load_naive_bayes()
{
    BayesModel* model = malloc(sizeof(BayesModel));
    FILE* file = fopen("assets/bayes_model.dat", "rb");

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
    TraceLog(LOG_INFO, "Model loaded from bayes_model.dat\n");

    return model;
}

void forward_pass(NeuralNetwork* nn, const double input[])
{
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        nn->hidden_layer[i] = nn->bias_hidden[i];
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->hidden_layer[i] += nn->hidden_weights[i][j] * input[j];
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

EvalResult nn_move(NeuralNetwork* nn)
{
    int best_move = -1;
    double best_score = -1;

    const uint16_t occupied = x_board | o_board;
    uint16_t legal_moves = ~occupied & 0b111111111;

    while (legal_moves)
    {
        const int move = __builtin_ctz(legal_moves);
        o_board |= 1 << move; // Try move

        double input[INPUT_NODES];
        for (int i = 0; i < INPUT_NODES; i++)
        {
            if (x_board & 1 << i) input[i] = 1.0;
            else if (o_board & 1 << i) input[i] = -1.0;
            else input[i] = 0.0;
        }

        forward_pass(nn, input);
        const double score = nn->output_layer[0];

        o_board &= ~(1 << move); // Undo move

        if (score > best_score)
        {
            best_score = score;
            best_move = move;
        }

        legal_moves &= ~(1 << move);
    }

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
        const int move = __builtin_ctz(legal_moves); // Get the least significant bit

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
EvalResult minimax(const player_t current_player, int alpha, int beta, const int depth, const GameContext* context)
{
    const player_t human = get_human_player(context);
    const player_t computer = get_computer_player(context);
    // Check win conditions
    if (check_win(human) != -1) return (EvalResult){-1, -1};
    if (check_win(computer) != -1) return (EvalResult){1, -1};
    if (check_draw() || depth == 0) return (EvalResult){0, -1};

    int bestScore = (current_player == computer) ? -2 : 2;
    int bestMove = -1;

    const uint16_t occupied_board = x_board | o_board;
    uint16_t legal_moves = ~occupied_board & 0b111111111;

    while (legal_moves)
    {
        const int move = __builtin_ctz(legal_moves);
        const int row = move / 3;
        const int col = move % 3;

        // try the move
        set_cell(row, col, current_player);

        const EvalResult result = minimax(
            current_player == human ? computer : human, alpha, beta, depth -1, context
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
        // maximizing for computer
        if (current_player == computer) {
            if (result.score > bestScore) {
                bestScore = result.score;
                bestMove = move;
            }
            alpha = (alpha > bestScore) ? alpha : bestScore;
        } else {
            if (result.score < bestScore) {
                bestScore = result.score;
                bestMove = move;
            }
            beta = (beta < bestScore) ? beta : bestScore;
        }

        if (alpha >= beta) {
            break;
        }

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
    case ONE_PLAYER_EASY:
        result = nn_move(models->neural_network);
        break;

    case ONE_PLAYER_EASY_NAIVE:
        result = nb_move(models->bayes_model, computer_player);
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

