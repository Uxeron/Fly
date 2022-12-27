#include "fly_white.h"
#include <cmath>

Vector FlyWhite::move(int window_x, int window_y, int mouse_x, int mouse_y)
{
    Vector mouse_pos = Vector(mouse_x, mouse_y);
    Vector position = Vector(distance, 0.0).rotated(rotation);
    rotation += speed;

    return mouse_pos + position - Vector(size/2, size/2);
}