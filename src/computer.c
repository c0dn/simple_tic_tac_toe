#include "computer.h"

/**
 * @brief Minimax algorithm for AI decision making
 *
 * Recursively evaluates game tree to find optimal move
 *
 * @param current_player Current player (X or O)
 * @return EvalResult containing score and best move
 *
 * Algorithm characteristics:
 * - Recursive depth-first search
 * - Maximizes computer's score
 * - Minimizes player's score
 * - Handles win, lose, and draw scenarios
 */
EvalResult minimax(const player_t current_player, const GameContext* context) {
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

        // Make the move
        set_cell(row, col, current_player);

        // Recursive call with the next player
        const EvalResult result = minimax(
            (current_player == PLAYER_X ? PLAYER_O : PLAYER_X), context
        );

        // Undo the move
        if (current_player == PLAYER_X) {
            x_board &= ~BIT_POS(row, col);
        } else {
            o_board &= ~BIT_POS(row, col);
        }

        // Update best score and move based on the current player
        if ((current_player == PLAYER_O && result.score > bestScore) ||
            (current_player == PLAYER_X && result.score < bestScore)) {
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
 */
void computer_move(const GameContext* context) {
    const player_t computer_player = get_computer_player(context);
    const EvalResult result = minimax(computer_player, context);

    // TODO: Add support for other algorithms here, switch between various game mode set in context
    if (result.move != -1) {
        const int row = result.move / 3;
        const int col = result.move % 3;
        set_cell(row, col, computer_player);
    }
}

