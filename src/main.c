#include <game.h>
#include <notcurses/notcurses.h>
#include <locale.h>
#include <render.h>
#include <handlers.h>
#include <uchar.h>

int main(void) {
    setlocale(LC_ALL, "");

    const notcurses_options opts = {
    };

    FILE* app_log = fopen("app.log", "w");

    if (app_log == NULL) {
        fprintf(stderr, "Failed to open application log file.\n");
        return EXIT_FAILURE;
    }


    struct notcurses* nc = notcurses_init(&opts, NULL);
    if (nc == NULL) {
        fclose(app_log);
        fprintf(stderr, "Failed to initialize notcurses.\n");
        return EXIT_FAILURE;
    }

    notcurses_cursor_disable(nc);
    notcurses_mice_enable(nc, NCMICE_ALL_EVENTS);


    struct ncplane* default_plane = notcurses_stdplane(nc);

    render_grid(default_plane);


    if (!notcurses_canutf8(nc)) {
        fprintf(stderr, "Terminal does not support UTF-8 encoding.\n");
        cleanup_and_stop(nc, app_log);
        return EXIT_FAILURE;
    }

    if (notcurses_render(nc) != 0) {
        fprintf(stderr, "Failed to render\n");
        cleanup_and_stop(nc, app_log);
        return EXIT_FAILURE;
    }

    GameState current_game_state = GAME_STATE_MENU;

    initialize_game(&current_game_state);

    ncinput ni;

        while (current_game_state != GAME_STATE_EXIT) {
        if (notcurses_get_blocking(nc, &ni) == (char32_t)-1) {
            // Error or EOF
            break;
        }

        if (ni.id == 'q' || ni.id == 'Q' || ni.id == NCKEY_ESC) {
            // Exit on q or Esc
            current_game_state = GAME_STATE_EXIT;
            continue;
        }

        switch (current_game_state) {
            case GAME_STATE_PLAYING:
                if (ni.id == NCKEY_BUTTON1 && ni.evtype == NCTYPE_PRESS) {
                    const int mouse_y = ni.y;
                    const int mouse_x = ni.x;

                    fprintf(app_log, "Mouse click x: %d, y: %d\n", mouse_x, mouse_y);
                    handle_mouse_click(default_plane, mouse_y, mouse_x, app_log, &current_game_state);
                    notcurses_render(nc);
                }
                break;

            case GAME_STATE_P1_WIN:
            case GAME_STATE_P2_WIN:
            case GAME_STATE_DRAW:
                if (current_game_state == GAME_STATE_P1_WIN) {
                    display_message(default_plane, L"Player 1 Wins!");
                } else if (current_game_state == GAME_STATE_P2_WIN) {
                    display_message(default_plane, L"Player 2 Wins!");
                } else if (current_game_state == GAME_STATE_DRAW) {
                    display_message(default_plane, L"It's a Draw!");
                }
                notcurses_render(nc);

                if (notcurses_get_blocking(nc, &ni) == (char32_t)-1) {
                    break;
                }

                if (ni.id == 'r' || ni.id == 'R') {
                    // Restart the game
                    initialize_game(&current_game_state);
                    ncplane_erase(default_plane);
                    render_grid(default_plane);
                    notcurses_render(nc);
                } else if (ni.id == 'q' || ni.id == 'Q' || ni.id == NCKEY_ESC) {
                    // Quit
                    current_game_state = GAME_STATE_EXIT;
                }
                break;

            default:
                break;
        }
    }

    cleanup_and_stop(nc, app_log);
    return EXIT_SUCCESS;
}
