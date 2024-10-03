#include <notcurses/notcurses.h>
#include <locale.h>

int main(void) {
    setlocale(LC_ALL, "");

    const notcurses_options opts = {};
    struct notcurses* nc = notcurses_init(&opts, NULL);
    if (nc == NULL) {
        return EXIT_FAILURE;
    }


    struct ncplane* default_plane = notcurses_stdplane(nc);

    // Tic Tae Toe grid options

    const int start_y = 2;
    const int start_x = 5;
    const int cell_width = 6;
    const int cell_height = 3;
    const int grid_width = cell_width * 3;
    const int grid_height = cell_height * 3;

    // Check if the terminal supports Unicode
    if (!notcurses_canutf8(nc)) {
        fprintf(stderr, "Terminal does not support UTF-8 encoding.\n");
        notcurses_stop(nc);
        return EXIT_FAILURE;
    }

    // Draw vertical lines
    for (int i = 1; i <= 2; ++i) {
        const int x = start_x + i * cell_width;
        for (int y = start_y; y <= start_y + grid_height; ++y) {
            if (ncplane_putwc_yx(default_plane, y, x, L'│') < 0) {
                fprintf(stderr, "Failed to draw vertical line at (%d, %d)\n", y, x);
            }
        }
    }

    // Draw horizontal lines
    for (int i = 1; i <= 2; ++i) {
        const int y = start_y + i * cell_height;
        for (int x = start_x; x <= start_x + grid_width; ++x)
        {
            if (ncplane_putwc_yx(default_plane, y, x, L'─') < 0) {
                fprintf(stderr, "Failed to draw horizontal line at (%d, %d)\n", y, x);
            }
        }
    }

    // Draw intersections
    for (int i = 1; i <= 2; ++i) {
        const int y = start_y + i * cell_height;
        for (int j = 1; j <= 2; ++j) {
            const int x = start_x + j * cell_width;
            if (ncplane_putwc_yx(default_plane, y, x, L'┼') < 0) {
                fprintf(stderr, "Failed to draw intersection at (%d, %d)\n", y, x);
            }
        }
    }

    if (notcurses_render(nc) != 0) {
        fprintf(stderr, "Failed to render\n");
        notcurses_stop(nc);
        return EXIT_FAILURE;
    }

    // Wait for a key press before exit
    ncinput ni;
    notcurses_get_blocking(nc, &ni);

    notcurses_stop(nc);
    return EXIT_SUCCESS;
}
