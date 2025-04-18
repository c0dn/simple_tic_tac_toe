#ifndef UTILS_H
#define UTILS_H
#include <buttons.h>
#include <raylib.h>

Rectangle calculate_button_rectangle(float btn_width, ComponentPadding btn_padding, float btn_height,
                                     float first_button_offset, int index, int buttons_per_row, int screen_height,
                                     int screen_width);

BoxDimensions calculate_centered_box_dimensions(float width_percentage, float height_percentage, int screen_height, int screen_width, MemoCache* cache);

Coords calculate_centered_text_xy(
    const char* message,
    int font_size,
    float ref_x,
    float ref_y,
    float ref_width,
    float ref_height
);

Coords calculate_text_xy_offset(
    const char* message,
    int font_size,
    float ref_x,
    float ref_y,
    float ref_width,
    float ref_height,
    float vertical_offset_percent,
    float horizontal_offset_percent, MemoCache* cache
);

int count_trailing_zeros(uint16_t x);

#endif //UTILS_H
