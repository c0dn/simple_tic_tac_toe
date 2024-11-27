#include "utils.h"

#include <common.h>
#include <raylib.h>

/**
 * Calculates centered box dimensions
 *
 * @param width_percentage Horizontal size of box relative to screen width (0.0 - 1.0)
 * @param height_percentage Vertical size of box relative to screen height (0.0 - 1.0)
 * @param screen_height Total screen height in pixels
 * @param screen_width Total screen width in pixels
 * @param cache Memoization cache to store and retrieve calculated dimensions
 *
 * @return BoxDimensions struct containing box width, height, x, and y coordinates
 *
 * @note Uses memoization to cache and quickly retrieve previously calculated dimensions
 * @note Positions box at the center of the screen
 */
BoxDimensions calculate_centered_box_dimensions(const float width_percentage, const float height_percentage,
                                                const int screen_height, const int screen_width, MemoCache
                                                * cache)
{
    // Create key
    BoxKey key = {
        .width_percentage = width_percentage,
        .height_percentage = height_percentage,
        .screen_height = screen_height,
        .screen_width = screen_width
    };

    // Find entry in cache
    BoxCache* entry = NULL;
    HASH_FIND(hh, cache->box_cache, &key, sizeof(BoxKey), entry);

    if (entry)
    {
        // If entry found return it
        return entry->result;
    }

    // Calculate
    BoxDimensions box = {
        .width = (float)screen_width * width_percentage,
        .height = (float)screen_height * height_percentage,
        .x = ((float)screen_width - ((float)screen_width * width_percentage)) / 2,
        .y = ((float)screen_height - ((float)screen_height * height_percentage)) / 2
    };

    // Saves entry after calculating
    BoxCache* new_entry = malloc(sizeof(BoxCache));
    if (new_entry) {
        new_entry->key = key;
        new_entry->result = box;
        HASH_ADD(hh, cache->box_cache, key, sizeof(BoxKey), new_entry);
    }


    return box;
}

/**
 * Calculates rectangle position for a button in a grid layout
 *
 * @param btn_width Width of each button
 * @param btn_padding Padding configuration for buttons
 * @param btn_height Height of each button
 * @param first_button_offset Vertical offset for the initial button row
 * @param index Current button index in the grid
 * @param buttons_per_row Number of buttons displayed horizontally
 * @param screen_height Total screen height in pixels
 * @param screen_width Total screen width in pixels
 *
 * @return Rectangle struct with button's x, y, width, and height
 */
Rectangle calculate_button_rectangle(const float btn_width, const ComponentPadding btn_padding, const float btn_height,
                                     const float first_button_offset, const int index, const int buttons_per_row,
                                     const int screen_height, const int screen_width

)
{

    const float btn_spacing = btn_padding.up + btn_padding.down;
    const float horizontal_spacing = btn_padding.left + btn_padding.right;
    const float start_y = (float)screen_height / 2 - btn_height / 2 + first_button_offset;

    // Calculate total width of all buttons in a row including spacing
    const float total_row_width =
        (float)buttons_per_row * btn_width + ((float)buttons_per_row - 1) * horizontal_spacing;
    const float base_x = (float)screen_width / 2.0f - total_row_width / 2;

    // Calculate row and column based on buttons_per_row
    const int row = index / buttons_per_row;
    const int col = index % buttons_per_row;

    const Rectangle result = {
        base_x + col * (btn_width + horizontal_spacing), start_y + row * (btn_height + btn_spacing),
        btn_width, btn_height
    };
    return result;
}

/**
 * Calculates centered text coordinates within a reference rectangle
 *
 * @param message Text string to be centered
 * @param font_size Size of the font used for rendering
 * @param ref_x X-coordinate of reference rectangle's top-left corner
 * @param ref_y Y-coordinate of reference rectangle's top-left corner
 * @param ref_width Width of reference rectangle
 * @param ref_height Height of reference rectangle
 *
 * @return Coords struct with calculated x and y for positioning
 */
Coords calculate_centered_text_xy(const char* message, const int font_size, const float ref_x, const float ref_y,
                                  const float ref_width, const float ref_height)
{
    const int text_width = MeasureText(message, font_size);
    const Coords coords = {
        .x = ref_x + (ref_width - (float)text_width) / 2,
        .y = ref_y + (ref_height - (float)font_size) / 2
    };
    return coords;
}


/**
 * Calculates text coordinates with percentage-based offsets within a reference rectangle
 *
 * @param message Text string to be positioned
 * @param font_size Size of the font used for rendering
 * @param ref_x X-coordinate of reference rectangle's top-left corner
 * @param ref_y Y-coordinate of reference rectangle's top-left corner
 * @param ref_width Width of reference rectangle
 * @param ref_height Height of reference rectangle
 * @param vertical_offset_percent Vertical offset percentage within reference rectangle (0.0 - 1.0)
 * @param horizontal_offset_percent Horizontal offset percentage within reference rectangle (0.0 - 1.0)
 * @param cache Optional memoization cache (unused in this implementation)
 *
 * @return Coords struct with calculated x and y for positioning
 */
Coords calculate_text_xy_offset(const char* message, const int font_size, const float ref_x, const float ref_y,
                                const float ref_width, const float ref_height, const float vertical_offset_percent,
                                const float horizontal_offset_percent, MemoCache* cache)
{
    const int text_width = MeasureText(message, font_size);
    const Coords coords = {
        .x = ref_x + ref_width * horizontal_offset_percent - (float)text_width / 2,
        .y = ref_y + ref_height * vertical_offset_percent - (float)font_size / 2
    };
    return coords;
}

/**
 * Counts the number of trailing zero bits in a 16-bit unsigned integer
 *
 * @param x Input 16-bit unsigned integer
 *
 * @return Number of trailing zero bits
 *         Returns 16 if input is zero
 *
 * @note Uses a simple bit-shifting algorithm
 * @note Time complexity is O(k), where k is the number of trailing zeros
 */
int count_trailing_zeros(uint16_t x)
{
    if (x == 0)
        return 16;

    int count = 0;
    while (!(x & 1))
    {
        x >>= 1;
        count++;
    }
    return count;
}
