#ifndef COMPUTER_H
#define COMPUTER_H
#include <game.h>


typedef struct {
    int score;
    int move;
} EvalResult;

EvalResult minimax(const player_t current_player, const GameContext* context);

void computer_move(const GameContext* context);



#endif //COMPUTER_H
