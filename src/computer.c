
#include "C:\Users\limxu\OneDrive\Documents\a SIT Y1.1\Group Project\simple_tic_tac_toe\include\computer.h"
#include "C:\Users\limxu\OneDrive\Documents\a SIT Y1.1\Group Project\simple_tic_tac_toe\include\neuralnetwork.h"  // Include neural network header

void load_model(NeuralNetwork *nn, const char *nn_weights)
{
    FILE *file = fopen(nn_weights, "rb");
    if (file == NULL)
    {
        printf("Failed to open file for loading model.\n");
        return;
    }

    // Read weights and biases from the file and load them into the network
    fread(nn->weights, sizeof(double), HIDDEN_NODES * INPUT_NODES, file);
    fread(nn->bias_hidden, sizeof(double), HIDDEN_NODES, file);
    fread(nn->output_weights, sizeof(double), OUTPUT_NODES * HIDDEN_NODES, file);
    fread(nn->bias_output, sizeof(double), OUTPUT_NODES, file);

    fclose(file);
    printf("Model loaded successfully from '%s'.\n", nn_weights);
}

void initialize_network(NeuralNetwork *nn)
{
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
        nn->bias_hidden[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }

    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->output_weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
        nn->bias_output[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }

    // Initialize Adam-specific moment terms to 0
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->m_weights[i][j] = 0.0;
            nn->v_weights[i][j] = 0.0;
        }
        nn->m_bias_hidden[i] = 0.0;
        nn->v_bias_hidden[i] = 0.0;
    }

    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->m_output_weights[i][j] = 0.0;
            nn->v_output_weights[i][j] = 0.0;
        }
        nn->m_bias_output[i] = 0.0;
        nn->v_bias_output[i] = 0.0;
    }
}

void forward_pass(NeuralNetwork *nn, double input[])
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

void nn_move(NeuralNetwork *nn, int board[INPUT_NODES]) {
    int best_move = -1;
    double best_score = -1;

    for (int i = 0; i < INPUT_NODES; i++) {
        if (board[i] == EMPTY) {
            board[i] = PLAYER_O; 
            double input[INPUT_NODES];
            for (int j = 0; j < INPUT_NODES; j++) {
                input[j] = (board[j] == PLAYER_X) ? 1.0 : (board[j] == PLAYER_O) ? -1.0 : 0.0;
            }

            forward_pass(nn, input);
            double score = nn->output_layer[0];

            board[i] = EMPTY;

            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }

    if (best_move != -1) {
        board[best_move] = PLAYER_O;
    }
}


EvalResult minimax(const player_t current_player) {
    // Check win conditions
    if (check_win(PLAYER_X)) return (EvalResult){-1, -1};
    if (check_win(PLAYER_O)) return (EvalResult){1, -1};
    if (check_draw()) return (EvalResult){0, -1};

    int bestScore = (current_player == PLAYER_O) ? -2 : 2;
    int bestMove = -1;

    const uint16_t occupied_board = x_board | o_board;
    uint16_t legal_moves = ~occupied_board & 0b111111111;

    while (legal_moves) {
        const int move = __builtin_ctz(legal_moves);
        const int row = move / 3;
        const int col = move % 3;

        set_cell(row, col, current_player);

        const EvalResult result = minimax(
            current_player == PLAYER_X ? PLAYER_O : PLAYER_X
        );

        // Undo move
        if (current_player == PLAYER_X) {
            x_board &= ~BIT_POS(row, col);
        } else {
            o_board &= ~BIT_POS(row, col);
        }

        // Update best score and move
        if ((current_player == PLAYER_O && result.score > bestScore) ||
            (current_player == PLAYER_X && result.score < bestScore)) {
            bestScore = result.score;
            bestMove = move;
            }

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
 */
void computer_move(const GameContext* context) {
    const player_t computer_player = get_computer_player(context);
    const EvalResult result = minimax(computer_player);
    // TODO: Add support for other algorithms here, switch between various game mode set in context
    if (result.move != -1) {
        const int row = result.move / 3;
        const int col = result.move % 3;
        set_cell(row, col, computer_player);
    }
}