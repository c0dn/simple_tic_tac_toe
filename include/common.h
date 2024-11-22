#ifndef COMMON_H
#define COMMON_H

#include <neural.h>
#include <raylib.h>
#include <stdint.h>
#include <uthash.h>

// Basic types and enums
typedef uint8_t player_t;
#define PLAYER_NONE 0
#define PLAYER_X 1
#define PLAYER_O 2

typedef enum {
    GAME_STATE_MENU,
    MENU_DIFF_CHOICE,
    GAME_STATE_PLAYING,
    GAME_STATE_P1_WIN,
    GAME_STATE_P2_WIN,
    GAME_STATE_DRAW,
    MENU_INSTRUCTIONS,
    MENU_SETTINGS,
    GAME_STATE_EXIT
} GameState;

typedef enum {
    TWO_PLAYER,
    ONE_PLAYER_EASY_NN,
    ONE_PLAYER_EASY_NAIVE,
    ONE_PLAYER_MEDIUM,
    ONE_PLAYER_HARD
} GameMode;

// UI-related structures
typedef struct {
    const float left;
    const float right;
    const float up;
    const float down;
} ComponentPadding;

typedef struct {
    float width;
    float height;
    float x;
    float y;
} BoxDimensions;

typedef struct {
    float x;
    float y;
} Coords;

// Game-related structures
typedef struct {
    double start_time;
    bool active;
} ActiveTransition;


typedef struct
{
    NeuralNetwork* neural_network;
    BayesModel* bayes_model;
} AiModels;

typedef struct {
    Music background_music;
    Sound fx_click;
    Sound fx_symbol;
    Sound fx_win;
    Sound fx_draw;
    Texture2D main_menu_img;
    Texture2D instructions_1;
    Texture2D music_on;
    Texture2D music_off;
    Texture2D instructions_2;
    AiModels* models;
} GameResources;

typedef struct {
    Color background_color;
    Color primary_btn_color;
    Color btn_clicked_color;
} UiOptions;

typedef struct {
    float width_percentage;
    float height_percentage;
    int screen_height;
    int screen_width;
} BoxKey;

typedef struct {
    BoxKey key;
    BoxDimensions result;
    UT_hash_handle hh;
} BoxCache;

typedef struct {
    BoxCache* box_cache;
} MemoCache;

typedef struct {
    float grid_size;
    int cell_size;
    int start_x;
    int start_y;
} GridDimensions;

typedef struct {
    bool needs_recalculation;
    bool exit_flag;
    GameState state;
    GameMode selected_game_mode;
    player_t player_1;
    bool computer_enabled;
    bool audio_disabled;
    ActiveTransition transition;
    bool start_screen_shown;
    int p1_score;
    int p2_score;
    GridDimensions grid;
    MemoCache* memo_cache;
} GameContext;

#endif // COMMON_H