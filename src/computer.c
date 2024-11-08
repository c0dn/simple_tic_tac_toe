#include "computer.h"


MiniMaxResult minimax(const player_t current_player) {
    // Check win conditions
    if (check_win(PLAYER_X)) return (MiniMaxResult){-1, -1};
    if (check_win(PLAYER_O)) return (MiniMaxResult){1, -1};
    if (check_draw()) return (MiniMaxResult){0, -1};

    int bestScore = (current_player == PLAYER_O) ? -2 : 2;
    int bestMove = -1;

    const uint16_t occupied_board = x_board | o_board;
    uint16_t legal_moves = ~occupied_board & 0b111111111;

    while (legal_moves) {
        const int move = __builtin_ctz(legal_moves);
        const int row = move / 3;
        const int col = move % 3;

        set_cell(row, col, current_player);

        const MiniMaxResult result = minimax(
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

    return (MiniMaxResult){bestScore, bestMove};
}

void computer_move(const GameContext* context) {
    const player_t computer_player = get_computer_player(context);
    const MiniMaxResult result = minimax(computer_player);

    if (result.move != -1) {
        const int row = result.move / 3;
        const int col = result.move % 3;
        set_cell(row, col, computer_player);
    }
}

