#include "buttons.h"

// Button click handlers
static void start_easy_mode(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void start_medium_mode(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void start_hard_mode(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void return_to_menu(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    *game_state = GAME_STATE_MENU;
}

static void start_1player(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    *game_state = MENU_DIFF_CHOICE;
}

static void start_2player(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    initialize_game(res, game_state);
    *game_state = GAME_STATE_PLAYING;
}

static void show_instructions(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    *game_state = MENU_INSTRUCTIONS;
}

static void show_settings(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    *game_state = MENU_SETTINGS;
}

static void show_exit_confirmation(const GameResources* res, GameState* game_state)
{
    PlaySound(res->fx_click);
    *game_state = GAME_STATE_EXIT;
}


Button SETTINGS_BUTTONS[4] = {
    {
        .text = "Play",
        .color = LIGHTGRAY,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -200.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
    },
    {
        .text = "Pause",
        .color = LIGHTGRAY,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -200.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
    },
    {
        .text = "Stop",
        .color = LIGHTGRAY,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -200.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
    },
    {
        .text = "Back",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -200.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30,
    }
};
Button GAME_MODE_BUTTONS[3] = {
    {
        .text = "Easy",
        .color = YELLOW,
        .hoverColor = DARKBLUE,
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
        .color = YELLOW,
        .hoverColor = DARKBLUE,
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
        .color = YELLOW,
        .hoverColor = DARKBLUE,
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
        .color = YELLOW,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 100.0f,
        .first_render_offset = -40.0f,
        .padding = PaddingY(10.0f),
        .rounded = true,
        .font_size = 30
    },
    {
        .text = "No",
        .color = YELLOW,
        .hoverColor = DARKBLUE,
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
        .color = BLUE,
        .hoverColor = DARKBLUE,
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
        .text = "Restart Game",
        .color = YELLOW,
        .hoverColor = DARKBLUE,
        .width = 200,
        .height = 60,
        .first_render_offset = 0,
        .padding = PaddingY(15.0f),
        .rounded = true,
        .font_size = 20,
        .action = initialize_game
    },
    {
        .text = "Back to Menu",
        .color = YELLOW,
        .hoverColor = DARKBLUE,
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
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 70.0f,
        .first_render_offset = -300.0f,
        .padding = PaddingY(30.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_1player
    },
    {
        .text = "2 Player",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 70.0f,
        .first_render_offset = -300.0f,
        .padding = PaddingY(30.0f),
        .rounded = true,
        .font_size = 30,
        .action = start_2player
    },
    {
        .text = "How To Play",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 70.0f,
        .first_render_offset = -300.0f,
        .padding = PaddingY(30.0f),
        .rounded = true,
        .font_size = 30,
        .action = show_instructions
    },
    {
        .text = "Settings",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 70.0f,
        .first_render_offset = -300.0f,
        .padding = PaddingY(30.0f),
        .rounded = true,
        .font_size = 30,
        .action = show_settings
    },
    {
        .text = "Exit",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .width = 330.0f,
        .height = 70.0f,
        .first_render_offset = -300.0f,
        .padding = PaddingY(30.0f),
        .rounded = true,
        .font_size = 30,
        .action = show_exit_confirmation
    }
};