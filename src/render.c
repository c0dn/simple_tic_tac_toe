#include "render.h"
#include <stdio.h>

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

void draw_symbol_in_cell(struct ncplane* n, const int cell_row, const int cell_column, const wchar_t symbol, FILE* app_log) {
    // Calculate the position to draw the symbol (center of the cell)
    const int symbol_y = START_Y + cell_row * CELL_HEIGHT + CELL_HEIGHT / 2;
    const int symbol_x = START_X + cell_column * CELL_WIDTH + CELL_WIDTH / 2;

    if (ncplane_putwc_yx(n, symbol_y, symbol_x, symbol) < 0) {
        fprintf(app_log, "Failed to draw symbol at (%d, %d)\n", symbol_y, symbol_x);
    }
}

void display_message(struct ncplane* n, const wchar_t* message) {
    ncplane_erase(n);

    unsigned int y_max, x_max;
    ncplane_dim_yx(n, &y_max, &x_max);

    const unsigned int msg_length = wcslen(message);
    const unsigned int msg_x = (x_max - msg_length) / 2;
    const unsigned int msg_y = y_max / 2;

    ncplane_putwstr_yx(n, msg_y, msg_x, message);
}
