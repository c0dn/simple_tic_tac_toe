#include "computer.h"


int minimax(const player_t current_player)
{
    // Check win conditions
    if (check_win(PLAYER_X)) return -1;
    if (check_win(PLAYER_O)) return 1;
    if (check_draw()) return 0;

    int bestScore = (current_player == PLAYER_O) ? -2 : 2;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_cell_empty(i, j)) {
                // Temporarily make the move
                set_cell(i, j, current_player);

                const int score = minimax(
                    current_player == PLAYER_X ? PLAYER_O : PLAYER_X
                );

                // Undo the move
                if (current_player == PLAYER_X) {
                    x_board &= ~BIT_POS(i, j);
                } else {
                    o_board &= ~BIT_POS(i, j);
                }

                // Update best score based on player
                if (current_player == PLAYER_O) {
                    bestScore = (score > bestScore) ? score : bestScore;
                } else {
                    bestScore = (score < bestScore) ? score : bestScore;
                }
            }
        }
    }
    return bestScore;
}

void computer_move(const GameContext* context) {
    int bestScore = -1000;
    int bestMove[2] = {-1, -1};
    const player_t computer_player = get_computer_player(context);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_cell_empty(i, j)) {
                // Temporarily make the move
                set_cell(i, j, computer_player);

                const int score = minimax(computer_player == PLAYER_X ? PLAYER_O : PLAYER_X);

                // Undo the move
                if (computer_player == PLAYER_X) {
                    x_board &= ~BIT_POS(i, j);
                } else {
                    o_board &= ~BIT_POS(i, j);
                }

                if (score > bestScore) {
                    bestScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }

    // Make the best move
    if (bestMove[0] != -1 && bestMove[1] != -1) {
        set_cell(bestMove[0], bestMove[1], computer_player);
    }
}