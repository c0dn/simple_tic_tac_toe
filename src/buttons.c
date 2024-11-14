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

static void return_to_menu(const GameResources* res, GameContext* context)
{
    PlaySound(res->fx_click);
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
        .text = "Easy",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -50.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_easy_mode
    },
    {
        .text = "Medium",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -50.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_medium_mode
    },
    {
        .text = "Hard",
        .width = 300.0f,
        .height = 50.0f,
        .first_render_offset = -50.0f,
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
        .action = initialize_game
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
