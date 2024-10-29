#include "render.h"
#include <stdio.h>

/**
 * @brief Renders a 3x3 grid on the given ncplane.
 *
 * This function draws a 3x3 grid using ASCII box-drawing characters.
 * The grid consists of vertical and horizontal lines, with intersections
 * at the crossing points.
 *
 * @param n Pointer to the ncplane on which to render the grid.
 *
 * @details
 * The grid is positioned using the following constants:
 * - START_X: The starting X coordinate of the grid
 * - START_Y: The starting Y coordinate of the grid
 * - CELL_WIDTH: The width of each cell in the grid
 * - CELL_HEIGHT: The height of each cell in the grid
 * - GRID_WIDTH: The total width of the grid
 * - GRID_HEIGHT: The total height of the grid
 *
 * The function draws:
 * 1. Vertical lines using the '│' character
 * 2. Horizontal lines using the '─' character
 * 3. Intersections using the '┼' character
 *
 * If any drawing operation fails, an error message is printed to stderr.
 */
void render_grid(struct ncplane* n) {
    // Draw vertical lines
    for (int i = 1; i <= 2; ++i) {
        const int x = START_X + i * CELL_WIDTH;
        for (int y = START_Y; y <= START_Y + GRID_HEIGHT; ++y) {
            if (ncplane_putwc_yx(n, y, x, L'│') < 0) {
                fprintf(stderr, "Failed to draw vertical line at (%d, %d)\n", y, x);
            }
        }
    }

    // Draw horizontal lines
    for (int i = 1; i <= 2; ++i) {
        const int y = START_Y + i * CELL_HEIGHT;
        for (int x = START_X; x <= START_X + GRID_WIDTH; ++x) {
            if (ncplane_putwc_yx(n, y, x, L'─') < 0) {
                fprintf(stderr, "Failed to draw horizontal line at (%d, %d)\n", y, x);
            }
        }
    }

    // Draw intersections
    for (int i = 1; i <= 2; ++i) {
        const int y = START_Y + i * CELL_HEIGHT;
        for (int j = 1; j <= 2; ++j) {
            const int x = START_X + j * CELL_WIDTH;
            if (ncplane_putwc_yx(n, y, x, L'┼') < 0) {
                fprintf(stderr, "Failed to draw intersection at (%d, %d)\n", y, x);
            }
        }
    }
}

/**
 * @brief Draws a symbol in the center of a specified cell.
 *
 * This function calculates the position to draw a symbol at the center of a given cell
 * and attempts to draw it using the ncurses plane.
 *
 * @param n The ncurses plane to draw on.
 * @param cell_row The row index of the cell.
 * @param cell_column The column index of the cell.
 * @param symbol The wide character symbol to be drawn.
 * @param app_log A file pointer for logging errors.
 */
void draw_symbol_in_cell(struct ncplane* n, const int cell_row, const int cell_column, const wchar_t symbol, FILE* app_log) {
    // Calculate the position to draw the symbol (center of the cell)
    const int symbol_y = START_Y + cell_row * CELL_HEIGHT + CELL_HEIGHT / 2;
    const int symbol_x = START_X + cell_column * CELL_WIDTH + CELL_WIDTH / 2;

    if (ncplane_putwc_yx(n, symbol_y, symbol_x, symbol) < 0) {
        fprintf(app_log, "Failed to draw symbol at (%d, %d)\n", symbol_y, symbol_x);
    }
}


/**
 * @brief Displays a centered message on the given ncplane.
 *
 * This function erases the contents of the given ncplane and displays
 * a new message centered both vertically and horizontally.
 *
 * @param n The ncplane on which to display the message.
 * @param message The wide character string message to be displayed.
 */
void display_message(struct ncplane* n, const wchar_t* message) {
    // Erase the contents of the plane
    ncplane_erase(n);

    // Get the dimensions of the plane
    unsigned int y_max, x_max;
    ncplane_dim_yx(n, &y_max, &x_max);

    // Calculate the position for centering the message
    const unsigned int msg_length = wcslen(message);
    const unsigned int msg_x = (x_max - msg_length) / 2;
    const unsigned int msg_y = y_max / 2;

    // Display the message at the calculated position
    ncplane_putwstr_yx(n, msg_y, msg_x, message);
}
