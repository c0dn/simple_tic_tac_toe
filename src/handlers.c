#include "handlers.h"

#include <game.h>
#include <render.h>

void cleanup_and_stop(struct notcurses* nc, FILE* app_log)
{
    notcurses_stop(nc);
    fclose(app_log);
}


void handle_mouse_click(struct ncplane* n, const int mouse_y, const int mouse_x, FILE* app_log, GameState *state) {

    if (*state != GAME_STATE_PLAYING) {
        return; // Ignore clicks if game not ongoing
    }


    const int relative_x = mouse_x - START_X;
    const int relative_y = mouse_y - START_Y;

    // Check if the click is within the grid boundaries, if outside do nothing
    if (relative_x < 0 || relative_y < 0) {
        fprintf(app_log, "Click outside the grid.\n");
        return;
    }

    // Determine the cell indices
    const int cell_column = relative_x / CELL_WIDTH;
    const int cell_row = relative_y / CELL_HEIGHT;

    if (cell_column >= 0 && cell_column < BOARD_SIZE &&
        cell_row >= 0 && cell_row < BOARD_SIZE) {
        fprintf(app_log, "Clicked cell row: %d, column: %d\n", cell_row, cell_column);

        if (is_cell_empty(cell_row, cell_column)) {
            board[cell_row][cell_column] = current_player_symbol;

            draw_symbol_in_cell(n, cell_row, cell_column, current_player_symbol, app_log);

            if (check_win(current_player_symbol)) {
                fprintf(app_log, "Player %lc wins!\n", current_player_symbol);

                if (current_player_symbol == L'X')
                    *state = GAME_STATE_P1_WIN;
                else
                    *state = GAME_STATE_P2_WIN;
            }
            else if (check_draw()) {
                fprintf(app_log, "Game is a draw.\n");
                *state = GAME_STATE_DRAW;
            }
            else {
                current_player_symbol = (current_player_symbol == L'X') ? L'O' : L'X';
            }
        } else {
            fprintf(app_log, "Cell is already occupied.\n");
        }
        } else {
            fprintf(app_log, "Click outside the grid cells.\n");
        }
}