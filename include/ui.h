#ifndef UI_H
#define UI_H

typedef struct
{
    const float left;
    const float right;
    const float up;
    const float down;
} ComponentPadding;



typedef struct
{
    float width;
    float height;
    float x;
    float y;
} BoxDimensions;

typedef struct
{
    float x;
    float y;
} Coords;

#endif //UI_H
