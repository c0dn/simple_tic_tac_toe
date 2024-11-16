#include "computer.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

// Probability matrices for each position in a winning or losing state
double probX[BOARD_SIZE][3], probO[BOARD_SIZE][3], probBlank[BOARD_SIZE][3];
double probWin, probLose;


int predict(const char board[BOARD_SIZE])
{
    double probX[BOARD_SIZE][3], probO[BOARD_SIZE][3], probBlank[BOARD_SIZE][3];
    double probWin = 0.5, probLose = 0.5; // Start with neutral prior probabilities

    int countWin = 0, countLose = 0;
    int countX[BOARD_SIZE][3] = {0}, countO[BOARD_SIZE][3] = {0}, countBlank[BOARD_SIZE][3] = {0};

    // Analyze the board to calculate weights dynamically
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i] == 'x') // If the cell is occupied by 'x'
        {
            countX[i][1]+=2; // Increase 'x' win count
            countO[i][0]-=1; // Reduce likelihood for 'o' in this position
            countWin++;     // Increment total win count
        }
        else if (board[i] == 'o') // If the cell is occupied by 'o'
        {
            countO[i][1]++; // Increase 'o' win count
            countX[i][0]-=1; // Reduce likelihood for 'x' in this position
            countLose++;    // Increment total loss count
        }
        else // If the cell is blank
        {
            countBlank[i][1]++;
            countBlank[i][0]++;
        }
    }

    // Compute probabilities with Laplace smoothing
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        probX[i][1] = (countX[i][1] + 1.0) / (countWin + 3.0);
        probX[i][0] = (countX[i][0] + 1.0) / (countLose + 3.0);
        probO[i][1] = (countO[i][1] + 1.0) / (countWin + 3.0);
        probO[i][0] = (countO[i][0] + 1.0) / (countLose + 3.0);
        probBlank[i][1] = (countBlank[i][1] + 1.0) / (countWin + 3.0);
        probBlank[i][0] = (countBlank[i][0] + 1.0) / (countLose + 3.0);
    }

    // Update overall win and loss probabilities
    probWin = (double)(countWin + 1) / (countWin + countLose + 2); // Add smoothing
    probLose = (double)(countLose + 1) / (countWin + countLose + 2);

    // Start with prior probabilities for prediction
    double winProb = probWin, loseProb = probLose;

    // Multiply conditional probabilities for each cell
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i] == 'x') // If the cell is occupied by 'x'
        {
            winProb *= probX[i][1];
            loseProb *= probX[i][0];
        }
        else if (board[i] == 'o') // If the cell is occupied by 'o'
        {
            winProb *= probO[i][1];
            loseProb *= probO[i][0];
        }
        else // If the cell is blank
        {
            winProb *= probBlank[i][1];
            loseProb *= probBlank[i][0];
        }
    }

    // Return the prediction (1 for win, 0 for lose)
    return winProb > loseProb ? 1 : 0;
}


NeuralNetwork *load_model()
{
    const static char weights_path[] = "assets/nn_weights.dat";
    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    FILE *file = fopen(weights_path, "rb");
    if (file == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to open file for loading model.\n");
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

// BayerProbabilities* init_naive_bayes()
// {
//     BayerProbabilities* probs = malloc(sizeof(BayerProbabilities));

//     probs->probWin = 0.6;
//     probs->probLose = 0.4;

//     for (int i = 0; i < BOARD_SIZE; i++)
//     {
//         probs->probX[i][1] = 0.6;
//         probs->probO[i][1] = 0.5;
//         probs->probBlank[i][1] = 0.8;

//         probs->probX[i][0] = 0.4;
//         probs->probO[i][0] = 0.3;
//         probs->probBlank[i][0] = 0.2;
//     }

//     return probs;
// }

void forward_pass(NeuralNetwork *nn, const double input[])
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

EvalResult nn_move(NeuralNetwork *nn)
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
            if (x_board & 1 << i)
                input[i] = 1.0;
            else if (o_board & 1 << i)
                input[i] = -1.0;
            else
                input[i] = 0.0;
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

// Naive Bayes move selection
EvalResult nb_move()
{
    int best_moves[BOARD_SIZE];                     // Array to store the best moves
    int best_move_count = 0;                        // Number of best moves
    double best_score = -1.0;                       // Keep track of the best score
    const uint16_t occupied = x_board | o_board;    // Get all occupied cells
    uint16_t legal_moves = ~occupied & 0b111111111; // Identify all legal moves
    // Evaluate each legal move
    while (legal_moves)
    {
        const int move = __builtin_ctz(legal_moves); // Get the least significant bit (lowest index)
        o_board |= (1 << move);                      // Simulate the move for 'o'
        // Create a board state to predict
        char board[BOARD_SIZE] = {0};
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            if (x_board & (1 << i))
                board[i] = 'x';
            else if (o_board & (1 << i))
                board[i] = 'o';
            else
                board[i] = 'b';
        }
        // Predict the outcome for this move
        int predicted_outcome = predict(board);
        // Assign a score based on the prediction
        double score = predicted_outcome == 1 ? 0.5 : 1.0; // Favor non-winning moves for the computer
        // Check if this move is better or equal to the best score
        if (score > best_score)
        {
            best_score = score;
            best_moves[0] = move; // Start a new list of best moves
            best_move_count = 1;  // Reset the count
        }
        else if (score == best_score)
        {
            best_moves[best_move_count++] = move; // Add to the list of best moves
        }
        o_board &= ~(1 << move);     // Undo the move
        legal_moves &= ~(1 << move); // Remove the move from legal moves
    }
    // Randomly select one of the best moves
    if (best_move_count > 0)
    {
        int selected_move = best_moves[rand() % best_move_count]; // Randomly select a move
        return (EvalResult){best_score, selected_move};
    }
    return (EvalResult){-1.0, -1}; // No valid moves
}

EvalResult minimax(const player_t current_player)
{
    // Check win conditions
    if (check_win(PLAYER_X) != -1)
        return (EvalResult){-1, -1};
    if (check_win(PLAYER_O) != -1)
        return (EvalResult){1, -1};
    if (check_draw())
        return (EvalResult){0, -1};

    int bestScore = (current_player == PLAYER_O) ? -2 : 2;
    int bestMove = -1;

    const uint16_t occupied_board = x_board | o_board;
    uint16_t legal_moves = ~occupied_board & 0b111111111;

    while (legal_moves)
    {
        const int move = __builtin_ctz(legal_moves);
        const int row = move / 3;
        const int col = move % 3;

        // Make the move
        set_cell(row, col, current_player);

        // Recursive call with the next player
        const EvalResult result = minimax(
            current_player == PLAYER_X ? PLAYER_O : PLAYER_X);

        // Undo move
        if (current_player == PLAYER_X)
        {
            x_board &= ~BIT_POS(row, col);
        }
        else
        {
            o_board &= ~BIT_POS(row, col);
        }

        // Update best score and move based on the current player
        if ((current_player == PLAYER_O && result.score > bestScore) ||
            (current_player == PLAYER_X && result.score < bestScore))
        {
            bestScore = result.score;
            bestMove = move;
        }

        // Remove the current move from legal_moves
        legal_moves &= ~(1 << move);
    }

    return (EvalResult){bestScore, bestMove};
}

/**
 * @brief Execute computer's move using minimax algorithm
 *
 * Selects and places optimal move on game board
 *
 * @param context Current game context
 * @param models
 */
void computer_move(const GameContext *context, const AiModels *models)
{
    const player_t computer_player = get_computer_player(context);
    EvalResult result;

    switch (context->selected_game_mode)
    {
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
    if (result.move != -1)
    {
        const int row = result.move / 3;
        const int col = result.move % 3;
        set_cell(row, col, computer_player);
    }
}
