#ifndef HANDLERS_H
#define HANDLERS_H
#include <game.h>
#include <notcurses/notcurses.h>

void cleanup_and_stop(struct notcurses* nc, FILE* app_log);


void handle_mouse_click(struct ncplane* n, int mouse_y, int mouse_x, FILE* app_log, GameState *state);

#endif //HANDLERS_H


