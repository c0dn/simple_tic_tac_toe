#include "utils.h"

#include <common.h>
#include <raylib.h>

BoxDimensions calculate_centered_box_dimensions(const float width_percentage, const float height_percentage,
                                                const int screen_height, const int screen_width, MemoCache
                                                * cache)
{
    BoxKey key = {
        .width_percentage = width_percentage,
        .height_percentage = height_percentage,
        .screen_height = screen_height,
        .screen_width = screen_width
    };

    BoxCache* entry = NULL;
    HASH_FIND(hh, cache->box_cache, &key, sizeof(BoxKey), entry);

    if (entry)
    {
        return entry->result;
    }

    BoxDimensions box = {
        .width = (float)screen_width * width_percentage,
        .height = (float)screen_height * height_percentage,
        .x = ((float)screen_width - ((float)screen_width * width_percentage)) / 2,
        .y = ((float)screen_height - ((float)screen_height * height_percentage)) / 2
    };

    BoxCache* new_entry = malloc(sizeof(BoxCache));
    if (new_entry) {
        new_entry->key = key;
        new_entry->result = box;
        HASH_ADD(hh, cache->box_cache, key, sizeof(BoxKey), new_entry);
    }


    return box;
}

Rectangle calculate_button_rectangle(const float btn_width, const ComponentPadding btn_padding, const float btn_height,
                                     const float first_button_offset, const int index, const int buttons_per_row,
                                     const int screen_height, const int screen_width, MemoCache* cache

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


Coords calculate_centered_text_xy(const char* message, const int font_size, const float ref_x, const float ref_y,
                                  const float ref_width, const float ref_height, MemoCache* cache)
{
    const int text_width = MeasureText(message, font_size);
    const Coords coords = {
        .x = ref_x + (ref_width - (float)text_width) / 2,
        .y = ref_y + (ref_height - (float)font_size) / 2
    };
    return coords;
}


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


int count_trailing_zeros(uint16_t x)
{
    if (x == 0)
        return 32;

    int count = 0;
    while (!(x & 1))
    {
        x >>= 1;
        count++;
    }
    return count;
}
