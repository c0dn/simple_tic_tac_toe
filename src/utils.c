#include "utils.h"
#include <raylib.h>

BoxDimensions calculate_centered_box_dimensions(const float width_percentage, const float height_percentage)
{
    BoxDimensions box;
    box.width = (float)GetScreenWidth() * width_percentage;
    box.height = (float)GetScreenHeight() * height_percentage;
    box.x = ((float)GetScreenWidth() - box.width) / 2;
    box.y = ((float)GetScreenHeight() - box.height) / 2;
    return box;
}

Rectangle calculate_button_rectangle(
    const float btn_width,
    const ComponentPadding btn_padding,
    const float btn_height,
    const float first_button_offset,
    const int index, const int buttons_per_row

) {
    const float btn_spacing = btn_padding.up + btn_padding.down;
    const float horizontal_spacing = btn_padding.left + btn_padding.right;
    const float start_y = (float)GetScreenHeight() / 2 - btn_height / 2 + first_button_offset;

    // Calculate total width of all buttons in a row including spacing
    const float total_row_width = (float)buttons_per_row * btn_width + ((float)buttons_per_row - 1) * horizontal_spacing;
    const float base_x = (float)GetScreenWidth() / 2.0f - total_row_width / 2;

    // Calculate row and column based on buttons_per_row
    const int row = index / buttons_per_row;
    const int col = index % buttons_per_row;

    return (Rectangle){
        base_x + col * (btn_width + horizontal_spacing),
        start_y + row * (btn_height + btn_spacing),
        btn_width,
        btn_height
    };
}


Coords calculate_centered_text_xy(
    const char* message,
    const int font_size,
    const float ref_x,
    const float ref_y,
    const float ref_width,
    const float ref_height
) {
    const int text_width = MeasureText(message, font_size);
    Coords cord;
    cord.x = ref_x + (ref_width - (float)text_width) / 2;
    cord.y = ref_y + (ref_height - (float)font_size) / 2;
    return cord;
}


Coords calculate_text_xy_offset(
    const char* message,
    const int font_size,
    const float ref_x,
    const float ref_y,
    const float ref_width,
    const float ref_height,
    const float vertical_offset_percent,
    const float horizontal_offset_percent
) {
    const int text_width = MeasureText(message, font_size);
    Coords cord;
    cord.x = ref_x + ref_width * horizontal_offset_percent - (float)text_width / 2;
    cord.y = ref_y + ref_height * vertical_offset_percent - (float)font_size / 2;
    return cord;
}


int count_trailing_zeros(uint16_t x) {
    if (x == 0) return 32;

    int count = 0;
    while (!(x & 1)) {
        x >>= 1;
        count++;
    }
    return count;
}