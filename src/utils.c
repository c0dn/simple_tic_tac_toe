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
    ButtonKey key = {
        .btn_width = btn_width,
        .btn_padding = btn_padding,
        .btn_height = btn_height,
        .first_button_offset = first_button_offset,
        .index = index,
        .buttons_per_row = buttons_per_row,
        .screen_height = screen_height,
        .screen_width = screen_width
    };

    ButtonCache* entry = NULL;
    HASH_FIND(hh, cache->button_cache, &key, sizeof(ButtonKey), entry);

    if (entry != NULL) {
        return entry->result;
    }

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

    Rectangle result = {
        base_x + col * (btn_width + horizontal_spacing), start_y + row * (btn_height + btn_spacing),
        btn_width, btn_height
    };

    ButtonCache* new_entry = malloc(sizeof(ButtonCache));
    if (new_entry) {
        memcpy(&new_entry->key, &key, sizeof(ButtonKey));
        memcpy(&new_entry->result, &result, sizeof(Rectangle));
        HASH_ADD(hh, cache->button_cache, key, sizeof(ButtonKey), new_entry);
    }

    return result;
}


Coords calculate_centered_text_xy(const char* message, const int font_size, const float ref_x, const float ref_y,
                                  const float ref_width, const float ref_height, MemoCache* cache)
{
    TextKey key = {
        .font_size = font_size,
        .ref_x = ref_x,
        .ref_y = ref_y,
        .ref_width = ref_width,
        .ref_height = ref_height
    };
    strncpy(key.message, message, sizeof(key.message) - 1);
    key.message[sizeof(key.message) - 1] = '\0';

    TextCache* entry = NULL;
    HASH_FIND(hh, cache->text_cache, &key, sizeof(TextKey), entry);

    if (entry != NULL) {
        return entry->result;
    }

    const int text_width = MeasureText(message, font_size);
    Coords coords = {
        .x = ref_x + (ref_width - (float)text_width) / 2,
        .y = ref_y + (ref_height - (float)font_size) / 2
    };

    TextCache* new_entry = malloc(sizeof(TextCache));
    if (new_entry) {
        new_entry->key = key;
        new_entry->result = coords;
        HASH_ADD(hh, cache->text_cache, key, sizeof(TextKey), new_entry);
    }
    return coords;
}


Coords calculate_text_xy_offset(const char* message, const int font_size, const float ref_x, const float ref_y,
                                const float ref_width, const float ref_height, const float vertical_offset_percent,
                                const float horizontal_offset_percent, MemoCache* cache)
{
    OffsetTextKey key = {
        .font_size = font_size,
        .ref_x = ref_x,
        .ref_y = ref_y,
        .ref_width = ref_width,
        .ref_height = ref_height,
        .vertical_offset_percent = vertical_offset_percent,
        .horizontal_offset_percent = horizontal_offset_percent
    };
    strncpy(key.message, message, sizeof(key.message) - 1);
    key.message[sizeof(key.message) - 1] = '\0';
    OffsetTextCache* entry = NULL;
    HASH_FIND(hh, cache->offset_text_cache, &key, sizeof(OffsetTextKey), entry);

    if (entry != NULL) {
        return entry->result;
    }

    const int text_width = MeasureText(message, font_size);
    Coords coords = {
        .x = ref_x + ref_width * horizontal_offset_percent - (float)text_width / 2,
        .y = ref_y + ref_height * vertical_offset_percent - (float)font_size / 2
    };

    OffsetTextCache* new_entry = malloc(sizeof(OffsetTextCache));
    if (new_entry) {
        new_entry->key = key;
        new_entry->result = coords;
        HASH_ADD(hh, cache->offset_text_cache, key, sizeof(OffsetTextKey), new_entry);
    }

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
