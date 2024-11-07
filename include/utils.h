#ifndef UTILS_H
#define UTILS_H
#include <raylib.h>
#include <render.h>

struct BoxDimensions
{
    float width;
    float height;
    float x;
    float y;
};

struct Cords
{
    float x;
    float y;
};

Rectangle calculate_button_rectangle(
    float btn_width,
    ComponentPadding btn_padding,
    float btn_height,
    float first_button_offset,
    int index
);

struct BoxDimensions calculate_centered_box_dimensions(float width_percentage, float height_percentage);

struct Cords calculate_centered_text_xy(
    const char* message,
    int font_size,
    float ref_x,
    float ref_y,
    float ref_width,
    float ref_height
);

struct Cords calculate_text_xy_offset(
    const char* message,
    int font_size,
    float ref_x,
    float ref_y,
    float ref_width,
    float ref_height,
    float vertical_offset_percent,
    float horizontal_offset_percent
);

#endif //UTILS_H
