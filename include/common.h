#ifndef COMMON_H
#define COMMON_H
#include <neural.h>
#include <raylib.h>
#include <ui.h>
#include <uthash.h>


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


typedef enum
{
    TWO_PLAYER,
    ONE_PLAYER_EASY,
    ONE_PLAYER_MEDIUM,
    ONE_PLAYER_HARD
} GameMode;

typedef struct {
    double start_time;
    double active;
} ActiveTransition;


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
    AiModels *models;
} GameResources;

typedef struct {
    Color background_color;
    Color primary_btn_color;
    Color btn_clicked_color;
} UiOptions;


typedef struct
{
    float btn_width;
    ComponentPadding btn_padding;
    float btn_height;
    float first_button_offset;
    int index;
    int buttons_per_row;
    int screen_height;
    int screen_width;
} ButtonKey;

typedef struct
{
    ButtonKey key;
    Rectangle result;
    UT_hash_handle hh;
} ButtonCache;


typedef struct
{
    float width_percentage;
    float height_percentage;
    int screen_height;
    int screen_width;
} BoxKey;


typedef struct
{
    BoxKey key;
    BoxDimensions result;
    UT_hash_handle hh;
} BoxCache;


typedef struct
{
    char message[512];
    int font_size;
    float ref_x;
    float ref_y;
    float ref_width;
    float ref_height;
} TextKey;

typedef struct
{
    TextKey key;
    Coords result;
    UT_hash_handle hh;
} TextCache;


typedef struct
{
    char message[256];
    int font_size;
    float ref_x;
    float ref_y;
    float ref_width;
    float ref_height;
    float vertical_offset_percent;
    float horizontal_offset_percent;
} OffsetTextKey;


typedef struct
{
    OffsetTextKey key;
    Coords result;
    UT_hash_handle hh;
} OffsetTextCache;

typedef struct
{
    ButtonCache* button_cache;
    BoxCache* box_cache;
} MemoCache;

typedef struct {
    bool needs_redraw;
    GameState state;
    GameMode selected_game_mode;
    player_t player_1;
    bool computer_enabled;
    bool audio_disabled;
    ActiveTransition transition;
    bool start_screen_shown;
    int p1_score;
    int p2_score;
    MemoCache* memo_cache;
} GameContext;

/**
 * @struct Button
 * @brief Represents a clickable button with configurable properties
 *
 * This structure defines a flexible button component with multiple rendering and interaction options.
 * It can be used across different menu screens and game states.
 */
typedef struct
{
    /**
     * @brief Rectangle defining the button's position and size
     *
     * Automatically calculated during rendering based on screen layout
     * and button configuration parameters
     */
    Rectangle rect;


    /**
     * @brief Text displayed on the button
     */
    const char* text;

    /**
     * @brief Button color
     *
     * Used when the button is not being hovered, required when override_default_colors is true
     */
    Color color;

    /**
     * @brief Width of the button in pixels
     */
    const float width;

    /**
     * @brief Height of the button in pixels
     */
    const float height;

    /**
     * @brief Initial vertical offset for first button rendering
     *
     * Helps in positioning the first button in a row or column
     */
    const float first_render_offset;

    /**
     * @brief Color when a button is clicked or hovered
     */
    const Color clickColor;

    /**
     * @brief Flag to override default rendering colors
     *
     * When true, uses button-specific colors instead of global theme colors
     */
    const bool override_default_colors;

    /**
     * @brief Padding configuration for button layout
     */
    const ComponentPadding padding;

    /**
     * @brief Determines if button corners should be rounded
     */
    const bool rounded;

    /**
     * @brief Font size for button text
     */
    const int font_size;

    /**
     * @brief Function pointer for button click action
     *
     * Callback function executed when the button is clicked
     * @param resources Pointer to game resources
     * @param context Pointer to current game context
     */
    void (*action)(const GameResources*, GameContext*);
} Button;

#endif //COMMON_H
