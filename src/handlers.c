#include "handlers.h"

#include <game.h>
#include <render.h>


/**
 * @brief Performs cleanup operations and stops the Notcurses interface.
 *
 * This function is responsible for gracefully shutting down the application.
 * It stops the Notcurses interface and closes the application log file.
 *
 * @param nc Pointer to the Notcurses context to be stopped.
 * @param app_log Pointer to the FILE stream of the application log to be closed.
 */
void cleanup_and_stop(struct notcurses* nc, FILE* app_log)
{
    notcurses_stop(nc);
    fclose(app_log);
}

/**
 * @brief Handles a mouse click event in the Tic-Tac-Toe game.
 *
 * This function processes a mouse click event, updating the game state and board
 * if the click is valid. It checks if the game is in progress, determines the clicked
 * cell, updates the board if the move is legal, checks for a win or draw condition,
 * and switches the current player if the game continues.
 *
 * @param n Pointer to the ncplane where the game is rendered.
 * @param mouse_y The y-coordinate of the mouse click.
 * @param mouse_x The x-coordinate of the mouse click.
 * @param app_log File pointer for logging game events.
 * @param state Pointer to the current game state.
 *
 * @note This function assumes the existence of global variables and functions such as:
 *       - START_X, START_Y: The starting coordinates of the game board.
 *       - CELL_WIDTH, CELL_HEIGHT: The dimensions of each cell in the game board.
 *       - BOARD_SIZE: The size of the game board (e.g., 3 for a 3x3 grid).
 *       - board: The 2D array representing the game board.
 *       - current_player_symbol: The symbol of the current player ('X' or 'O').
 *       - is_cell_empty(), draw_symbol_in_cell(), check_win(), check_draw(): Helper functions.
 *
 * @return This function does not return a value. It updates the game state and board directly.
 */
void handle_mouse_click(struct ncplane* n, const int mouse_y, const int mouse_x, FILE* app_log, GameState *state) {

    // Check game state before containing, stop execution if not in game
    if (*state != GAME_STATE_PLAYING) {
        return;
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