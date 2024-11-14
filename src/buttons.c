#include "buttons.h"

// Button click handlers
static void start_easy_mode(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_EASY;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}


static void start_easy_mode_naive(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_EASY_NAIVE;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

static void start_medium_mode(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_MEDIUM;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

static void start_hard_mode(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = ONE_PLAYER_HARD;
    context->computer_enabled = true;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

static void continue_playing(const GameResources* res, GameContext* context)
{
    context->player_1 = context->player_1 == PLAYER_X ? PLAYER_O : PLAYER_X;
    initialize_game(res,  context);
}


static void return_to_menu(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->p2_score = 0;
    context->p1_score = 0;
    context->state = GAME_STATE_MENU;
}

static void start_1player(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->state = MENU_DIFF_CHOICE;
}

static void start_2player(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->selected_game_mode = TWO_PLAYER;
    context->computer_enabled = false;
    initialize_game(res, context);
    context->state = GAME_STATE_PLAYING;
}

static void show_instructions(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->state = MENU_INSTRUCTIONS;
}

static void show_exit_confirmation(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
    context->state = GAME_STATE_EXIT;
}

Button GAME_MODE_BUTTONS[] = {
    {
        .text = "Easiest",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -75.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_easy_mode
    },
    {
        .text = "Easy",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -75.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_easy_mode_naive
    },
    {
        .text = "Medium",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -75.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_medium_mode
    },
    {
        .text = "Hard",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -75.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_hard_mode
    }
};

Button EXIT_CONFIRMATION_BUTTONS[] = {
    {
        .text = "Yes",
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -40.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30
    },
    {
        .text = "No",
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -40.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30
    }
};

Button INSTRUCTIONS_BUTTONS[] = {
    {
        .text = "Back",
        .width = 330.0f,
        .height = 100.0f,
        .padding = PaddingY(10.0f),
        .first_render_offset = 300.0f,
        .rounded = true,
        .font_size = 70,
        .action = return_to_menu
    }
};

Button GAME_OVER_BUTTONS[] = {
    {
        .text = "Continue Playing",
        .width = 200,
        .height = 60,
        .first_render_offset = 0,
        .padding = PaddingY(15.0f),
        .rounded = true,
        .font_size = 20,
        .action = continue_playing
    },
    {
        .text = "End Game",
        .width = 200,
        .height = 60,
        .first_render_offset = 0,
        .padding = PaddingY(15.0f),
        .rounded = true,
        .font_size = 20,
        .action = return_to_menu
    }
};


Button MAIN_MENU_BUTTONS[] = {
    {
        .text = "1 Player",
        .width = 200.0f,
        .height = 60.0f,
        .first_render_offset = 100.0f,
        .padding = {20, 20, 10, 10},
        .rounded = true,
        .font_size = 30,
        .action = start_1player
    },
    {
        .text = "2 Player",
        .width = 200.0f,
        .height = 60.0f,
        .first_render_offset = 100.0f,
        .padding = {20, 20, 10, 10},
        .rounded = true,
        .font_size = 30,
        .action = start_2player
    },
    {
        .text = "How To Play",
        .width = 200.0f,
        .height = 60.0f,
        .first_render_offset = 120.0f,
        .padding = {20, 20, 10, 10},
        .rounded = true,
        .font_size = 30,
        .action = show_instructions
    },
    {
        .text = "Exit",
        .width = 200.0f,
        .height = 60.0f,
        .first_render_offset = 120.0f,
        .padding = {20, 20, 10, 10},
        .rounded = true,
        .font_size = 30,
        .action = show_exit_confirmation
    }
};


Button IN_GAME_BUTTONS[] = {
    {
        .text = "Return to menu",
        .width = 330.0f,
        .height = 100.0f,
        .padding = PaddingY(10.0f),
        .first_render_offset = 400.0f,
        .rounded = true,
        .font_size = 40,
        .action = return_to_menu
    }
};
