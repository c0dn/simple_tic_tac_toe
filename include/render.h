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


typedef struct
{
    const float left;
    const float right;
    const float up;
    const float down;
} ComponentPadding;

// For vertical padding (up and down same)
#define PaddingY(value) (ComponentPadding){ 0, 0, value, value }

// For horizontal padding (left and right same)
#define PaddingX(value) (ComponentPadding){ value, value, 0, 0 }

// For uniform padding in all directions
#define Padding(value) (ComponentPadding){ value, value, value, value }


/**
 * @struct Button
 * @brief Represents a clickable button with configurable properties
 *
 * This structure defines a flexible button component with multiple rendering and interaction options.
 * It can be used across different menu screens and game states.
 */
typedef struct {
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



void render_grid(const GameResources* resources, const UiOptions* render_opts);
void render_menu(
    const GameResources* resources, const UiOptions* render_opts, const GameContext* context);
void render_game_over(
    const GameContext* context, const UiOptions* render_opts);
void render_instructions(
    const GameResources* resources, const UiOptions* render_opts);
void render_exit(const UiOptions* render_opts);
void render_game_mode_choice(const UiOptions* render_opts);
Rectangle calc_music_icon_rect(const GameContext* context, const GameResources* resources);
#endif //RENDER_H
