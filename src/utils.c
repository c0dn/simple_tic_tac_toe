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
    const int index
) {
    const float btn_spacing = btn_padding.up + btn_padding.down;
    const float start_y = (float)GetScreenHeight() / 2 - btn_height / 2 + first_button_offset;
    const float btn_x = (float)GetScreenWidth() / 2.0f - (btn_width + btn_padding.left + btn_padding.right) / 2.0f;

    return (Rectangle){
        btn_x + btn_padding.left,
        start_y + index * btn_spacing + index * btn_height,
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
