#ifndef BUTTONS_H
#define BUTTONS_H
#include <game.h>
#include <menu.h>


typedef struct
{
    const float left;
    const float right;
    const float up;
    const float down;
} ComponentPadding;

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


extern Button GAME_MODE_BUTTONS[3];
extern Button EXIT_CONFIRMATION_BUTTONS[2];
extern Button INSTRUCTIONS_BUTTONS[1];
extern Button MAIN_MENU_BUTTONS[4];
extern Button GAME_OVER_BUTTONS[2];
extern Button IN_GAME_BUTTONS[1];

#endif //BUTTONS_H
