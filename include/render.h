#ifndef RENDER_H
#define RENDER_H

#define START_Y 2
#define START_X 5
#define CELL_WIDTH 6
#define CELL_HEIGHT 3
#define GRID_WIDTH (CELL_WIDTH * 3)
#define GRID_HEIGHT (CELL_HEIGHT * 3)
#include <game.h>
#include <menu.h>



void render_grid(const GameResources* resources, const UiOptions* render_opts, const GameContext* context, bool show_buttons);
void render_menu(
    const GameResources* resources, const UiOptions* render_opts, const GameContext* context);
void render_game_over(
    const GameContext* context, const UiOptions* render_opts);
void render_instructions(
    const GameResources* resources, const UiOptions* render_opts, MemoCache* cache);
void render_exit(const UiOptions* render_opts, MemoCache* cache);
void render_game_mode_choice(const UiOptions* render_opts, MemoCache* cache);
Rectangle calc_music_icon_rect(const GameContext* context, const GameResources* resources);
void do_game_start_transition(const GameResources* resources, const UiOptions* render_opts,
                                  GameContext* context);
void do_game_over_transition(
 const GameResources* resources,
 const UiOptions* render_opts,
 GameContext* context
);

#endif //RENDER_H
