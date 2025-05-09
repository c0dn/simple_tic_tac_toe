#include <game.h>

uint16_t x_board;
uint16_t o_board;
player_t current_player;



/**
 * @brief Initializes a new game session
 *
 * @param res Pointer to game resources
 * @param context Pointer to game's current context data
 *
 * @details
 * This function resets the relevant variables to prep for the next game
 */
void initialize_game(const GameResources* res, GameContext* context)
{
    x_board = 0;
    o_board = 0;
    context->state = GAME_STATE_PLAYING;
    current_player = PLAYER_X;
    context->start_screen_shown = false;
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
 * @return the winning pattern otherwise returns -1, no winning patterns found
 *
 * @details
 * Uses predefined bit patterns representing winning combinations
 * - Compares player's bitboard against each winning pattern using bitwise AND
 * - Returns true if any winning pattern is fully matched
 * - Checks 8 possible win conditions
 */
int check_win(const player_t player)
{
    static const uint16_t WIN_PATTERNS[] = {
        0b000000111, // Row 1    (top row)
        0b000111000, // Row 2    (middle row)
        0b111000000, // Row 3    (bottom row)
        0b100100100, // Col 1    (left column)
        0b010010010, // Col 2    (middle column)
        0b001001001, // Col 3    (right column)
        0b100010001, // Diagonal (top left to bottom right)
        0b001010100  // Diagonal (top right to bottom left)
    };

    // Get the current player's board
    const uint16_t current = player == PLAYER_X ? x_board : o_board;

    // Check against each winning pattern
    for (int i = 0; i < 8; i++)
    {
        if ((current & WIN_PATTERNS[i]) == WIN_PATTERNS[i])
        {
            return i;
        }
    }

    return -1;
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
#ifndef USE_ASM_CHECK_DRAW
bool check_draw(void)
{
    return (x_board | o_board) == 0b111111111;
}
#endif

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
    if (x_board & pos)
        return PLAYER_X;
    if (o_board & pos)
        return PLAYER_O;
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
bool is_computer_win(const GameContext *context)
{
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
player_t get_human_player(const GameContext *context)
{
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
player_t get_computer_player(const GameContext *context)
{
    return context->player_1 == PLAYER_X ? PLAYER_O : PLAYER_X;
}


/**
 * @brief Updates the score in the game context based on the game state.
 *
 * @param state The current state of the game (draw, player 1 win, or player 2 win).
 * @param context Pointer to the GameContext structure containing the scores.
 */
void update_score(const GameState state, GameContext *context)
{
    switch (state)
    {
        case GAME_STATE_DRAW:
            context->draw_score++; // increment draw score
            break;

        case GAME_STATE_P1_WIN:
            context->p1_score++; // increment player 1 score
            break;

        case GAME_STATE_P2_WIN:
            context->p2_score++; // increment player2 or computer score
            break;

        default:
            // Invalid state - no action
            break;
    }
}


/**
 * @brief Updates the game state and scores based on the current game conditions.
 *
 * This function checks for a win or a draw condition, updates the game state,
 * and then updates the scores in the GameContext.
 *
 * @param context Pointer to the GameContext structure containing the game state and scores.
 */
void update_game_state_score(GameContext *context)
{
    const int result = check_win(current_player); // check for win

    if (result != -1) // if player won
    {
        if (current_player == context->player_1)
        {
            // game state is p1 win if it is current player
            context->state = GAME_STATE_P1_WIN;
        }
        else
        {
            // game state is p2 win if it is current player
            context->state = GAME_STATE_P2_WIN;
        }
    }
    else if (check_draw()) // check draw
    {
        // set game state to draw when all positions are filled with no winner
        context->state = GAME_STATE_DRAW;
    }

    // update the score based on game state
    update_score(context->state, context);
}


/**
 * @brief displays current scores
 *
 * Depending on whether the computer is enabled, this function displays the scores
 * for either one-player (human vs computer) or two-player mode. It also shows
 * the number of games that ended in a draw.
 *
 * @param context Pointer to the GameContext structure containing the scores and game mode.
 */
void display_score(const GameContext *context)
{
    if (context->computer_enabled)
    {
        // Display scores for one-player mode
        DrawText(TextFormat("Human: %d", context->p1_score), 10, 100, 40, BLACK);
        DrawText(TextFormat("Computer: %d", context->p2_score), 760, 100, 40, BLACK);
    }
    else
    {
        // Display scores for two-player mode
        DrawText(TextFormat("Player 1: %d", context->p1_score), 10, 100, 40, BLACK);
        DrawText(TextFormat("Player 2: %d", context->p2_score), 760, 100, 40, BLACK);
    }

    // Display the number of games that ended in a draw
    DrawText(TextFormat("Draws: %d", context->draw_score), 410, 100, 40, BLACK);
}