#include "handlers.h"

#include <game.h>
#include <render.h>

/**************************************************************/
/*  function: cleanup_and_stop                                */
/*                                                            */
/*  Purpose:                                                  */
/*     This function handles cleanup operations at the end of */
/*     the game or when the program needs to stop. It shuts   */
/*     down the `notcurses` instance and closes the log file. */
/*                                                            */
/*  Input Parameters:                                         */
/*     struct notcurses* nc - Pointer to the `notcurses`      */
/*                            instance                        */
/*     FILE* app_log        - File pointer to the application */
/*                            log                             */
/*                                                            */
/*  Returns:                                                  */
/*     void - This function does not return anything.         */
/*                                                            */
/**************************************************************/
void cleanup_and_stop(struct notcurses* nc, FILE* app_log)
{
    notcurses_stop(nc);
    fclose(app_log);
}

/**************************************************************/
/*  function: handle_mouse_click                              */
/*                                                            */
/*  Purpose:                                                  */
/*     This function handles mouse click events in the game.  */
/*     It determines if the click was inside the game board   */
/*     and processes the click based on the game state and    */
/*     current player. If the game is still playing, the      */
/*     function updates the game board, checks for a win or   */
/*     a draw, and switches the current player.               */
/*                                                            */
/*  Input Parameters:                                         */
/*     struct ncplane* n    - Pointer to the ncurses plane used */
/*                            for drawing.                    */
/*     const int mouse_y    - The y-coordinate of the mouse    */
/*                            click.                          */
/*     const int mouse_x    - The x-coordinate of the mouse    */
/*                            click.                          */
/*     FILE* app_log        - File pointer for logging game    */
/*                            events and diagnostics.         */
/*     GameState *state     - Pointer to the game state,       */
/*                            which is modified if the game    */
/*                            ends or continues.              */
/*                                                            */
/*  Returns:                                                  */
/*     void - This function doesn't return anything, but it   */
/*            modifies the game state and logs relevant       */
/*            events.                                         */
/*                                                            */
/**************************************************************/
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