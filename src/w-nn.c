// computer is player O

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BOARD_SIZE 3
#define BIT_POS(row, col) (1 << ((row) * 3 + (col)))

// Use 16-bit string to represent a player's board
uint16_t x_board;
uint16_t o_board;

typedef uint8_t player_t;
#define PLAYER_NONE 0
#define PLAYER_X 1
#define PLAYER_O 2

player_t current_player;

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
#define EXPORT_FILEPATH "C:\\Users\\limxu\\OneDrive\\Documents\\a SIT Y1.1\\Group Project\\simple_tic_tac_toe\\assets\\nn_weights.dat" // Path to the saved model file


/***************************************************************/
/* draw the tic tac toe board                                  */
/***************************************************************/
char gridChar(int row, int col) {
    if (x_board & BIT_POS(row, col)) {
        return 'X';
    } else if (o_board & BIT_POS(row, col)) {
        return 'O';
    } else {
        return ' ';
    }
}

void draw() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            printf(" %c ", gridChar(row, col));
            if (col < BOARD_SIZE - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (row < BOARD_SIZE - 1) {
            printf("---+---+---\n");
        }
    }
}

/***************************************************************/
/* william's code                                              */
/***************************************************************/
bool is_cell_empty(const int row, const int col)
{
    const uint16_t pos = BIT_POS(row, col);
    return !((x_board | o_board) & pos);
}

// modified this from check_win() to win()
bool win(const player_t player) {
    static const uint16_t WIN_PATTERNS[] = {
        0b000000111, // Row 1    (top row)
        0b000111000, // Row 2    (middle row)
        0b111000000, // Row 3    (bottom row)
        0b100100100, // Col 1    (left column)
        0b010010010, // Col 2    (middle column)
        0b001001001, // Col 3    (right column)
        0b100010001, // Diagonal (top left to bottom right)
        0b001010100 // Diagonal (top right to bottom left)
    };
    // Get the current player's board
    const uint16_t current = player == PLAYER_X ? x_board : o_board;

    // Check against each winning pattern
    for (int i = 0; i < 8; i++)
    {
        if ((current & WIN_PATTERNS[i]) == WIN_PATTERNS[i])
        {
            return true;
        }
    }

    return false;
}

bool check_draw(void)
{
    return (x_board | o_board) == 0b111111111;
}

player_t get_cell(const int row, const int col)
{
    const uint16_t pos = BIT_POS(row, col);
    if (x_board & pos) return PLAYER_X;
    if (o_board & pos) return PLAYER_O;
    return PLAYER_NONE;
}

void set_cell(const int row, const int col, const player_t player)
{
    const uint16_t pos = BIT_POS(row, col);
    if (player == PLAYER_X)
    {
        x_board |= pos;
    }
    else if (player == PLAYER_O)
    {
        o_board |= pos;
    }
}

/***************************************************************/
/*                     NEURAL NETWORK                          */
/***************************************************************/

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
        if (board[i] == PLAYER_NONE) {
            board[i] = PLAYER_O; // AI makes its move as O
            double input[INPUT_NODES];
            for (int j = 0; j < INPUT_NODES; j++) {
                input[j] = (board[j] == PLAYER_X) ? 1.0 : (board[j] == PLAYER_O) ? -1.0 : 0.0;
            }

            forward_pass(nn, input);
            double score = nn->output_layer[0]; // Assuming output layer provides a score for the AI's position

            board[i] = PLAYER_NONE;

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

/***************************************************************/
/* player's move                                               */
/***************************************************************/
void playerMove() {
    int row, col;
    while (true) { //when it is player's turn

        // get user input on where they want to place their mark
        // row input first
        printf("Enter your move (row: 0, 1 or 2): ");
        scanf("%d", &row);
        //column input
        printf("Enter your move (column: 0, 1 or 2): ");
        scanf("%d", &col);

        // check for validity of input
        // input range for row and column must be within 0 ro 2 and the cell needs to be empty
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && is_cell_empty(row, col)) {
            x_board |= BIT_POS(row, col); // place user's mark
            break;
        } else { // if input is out of range and cell already has a mark
            printf("Invalid move. Try again. \n");
        }
    }
}


/***************************************************************/
/* main function                                               */
/***************************************************************/
int main() {

    // Initialize the neural network and load model weights
    initialize_network(&nn);
    load_model(&nn, EXPORT_FILEPATH);

    // Initialize the game state
    x_board = 0;
    o_board = 0;
    current_player = PLAYER_X;

    printf("Welcome to Tic-Tac-Toe!\n");
    draw();

    // Game loop
    while (true) {
        if (current_player == PLAYER_X) {
            printf("Your turn (Player X).\n");
            playerMove();
            draw();

            if (win(PLAYER_X)) {
                printf("Congratulations! You win!\n");
                break;
            } else if (check_draw()) {
                printf("It's a draw!\n");
                break;
            }
            current_player = PLAYER_O;
        } else {
            printf("Computer's turn (Player O).\n");

            int board[INPUT_NODES];
            // Convert the board state to an input array for the neural network
            for (int i = 0; i < INPUT_NODES; i++) {
                int row = i / BOARD_SIZE;
                int col = i % BOARD_SIZE;
                board[i] = get_cell(row, col);
            }

            // Computer makes a move
            nn_move(&nn, board);

            // Update the board state based on the move made by the neural network
            for (int i = 0; i < INPUT_NODES; i++) {
                int row = i / BOARD_SIZE;
                int col = i % BOARD_SIZE;
                if (board[i] == PLAYER_O) {
                    set_cell(row, col, PLAYER_O);
                }
            }

            draw();

            if (win(PLAYER_O)) {
                printf("Computer wins!\n");
                break;
            } else if (check_draw()) {
                printf("It's a draw!\n");
                break;
            }
            current_player = PLAYER_X;
        }
    }

    printf("Game over.\n");
    return 0;
}
