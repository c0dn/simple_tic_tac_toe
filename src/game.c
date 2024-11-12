#include <game.h>

uint16_t x_board;
uint16_t o_board;
player_t current_player;
int p1_score =0;
int p2_score =0;

void initialize_game(const GameResources* res, GameContext* context)
{
    x_board = 0;
    o_board = 0;
    context->state = GAME_STATE_PLAYING;
    current_player = PLAYER_X;
}


/**
 * @brief Checks if a specific cell is empty using bitwise operations
 *
 * @param row Row index of cell to check (0-2)
 * @param col Column index of cell to check (0-2)
 * @return true if the cell is empty, false if occupied
 *
 * @details
 * Combine player boards and check if the cell is empty on the combined board
 * - Uses bitwise OR to combine both players' boards
 * - Uses bitwise AND with position mask to check specific cell
 * - Returns true if neither player occupies the position
 */
bool is_cell_empty(const int row, const int col)
{
    const uint16_t pos = BIT_POS(row, col);
    return !((x_board | o_board) & pos);
}

/**
 * @brief Checks if the specified player has won using bitwise operations
 *
 * @param player Player to check for win (PLAYER_X or PLAYER_O)
 * @return true if a player has won, false otherwise
 *
 * @details
 * Uses predefined bit patterns representing winning combinations
 * - Compares player's bitboard against each winning pattern using bitwise AND
 * - Returns true if any winning pattern is fully matched
 * - Checks 8 possible win conditions
 */
bool check_win(const player_t player)
{
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
            if (player == PLAYER_X){
                p1_score++;
            }
            else if(player == PLAYER_O)
            {
                p2_score++;
            }
            return true;
        }
    }
    return false;
}


/**
 * @brief Checks if the game is a draw using bitwise OR
 *
 * @return true if the game is drawn (all positions filled), false otherwise
 *
 * @details
 * Combines both players' boards using bitwise OR
 * - Compares result with full board pattern (0b111111111)
 * - Draw occurs when all nine spots are occupied (all bits set)
 */
bool check_draw(void)
{
    return (x_board | o_board) == 0b111111111;
}


/**
 * @brief Gets the player occupying a specific cell
 *
 * @param row The row index (0-2)
 * @param col The column index (0-2)
 * @return player_t PLAYER_NONE, PLAYER_X, or PLAYER_O
 */
player_t get_cell(const int row, const int col)
{
    const uint16_t pos = BIT_POS(row, col);
    if (x_board & pos) return PLAYER_X;
    if (o_board & pos) return PLAYER_O;
    return PLAYER_NONE;
}


/**
 * @brief Sets a cell to a specific player
 *
 * @param row The row index (0-2)
 * @param col The column index (0-2)
 * @param player The player making the move
 */
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

/**
 * @brief Checks if the computer has won the game
 *
 * @param context Pointer to the current game context
 * @return true if the computer is enabled and has won the game
 * @return false if the computer is not enabled or has not won
 *
 * @details
 * Verifies if computer mode is active before
 * - Checking if the game state indicates a Player 2 (computer) victory
 *
 * @note Assumes computer is always Player 2 in single-player modes
 */
bool is_computer_win(const GameContext* context) {
    return context->computer_enabled &&
           context->state == GAME_STATE_P2_WIN;
}

/**
 * @brief Determines the human player based on game context
 *
 * @param context Pointer to the current game context
 * @return player_t The player representing the human (either PLAYER_X or PLAYER_O)
 *
 * @note Returns the player set as player_1 in the game context
 */
player_t get_human_player(const GameContext* context) {
    return context->player_1;
}

/**
 * @brief Determines the computer player based on game context
 *
 * @param context Pointer to the current game context
 * @return player_t The player representing the computer (opposite of player_1)
 *
 * @note Returns the player not set as player_1 in the game context
 */
player_t get_computer_player(const GameContext* context) {
    return context->player_1 == PLAYER_X ? PLAYER_O : PLAYER_X;
}
void display_score()
{
    DrawText(TextFormat("Player 1: %d",p1_score), 10,0,40,BLACK);
    DrawText(TextFormat("Player 2: %d",p2_score), 760,0,40,BLACK);
}

void reset_score()
{
    p1_score =0;
    p2_score =0;
}
