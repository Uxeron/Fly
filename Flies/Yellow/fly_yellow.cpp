#include "fly_yellow.h"
#include <cmath>

Vector FlyYellow::move(int window_x, int window_y, int mouse_x, int mouse_y)
{
    if (velocity == nullptr)
        velocity = new Vector();

    Vector fly_pos = Vector(window_x + size/2, window_y + size/2);
    Vector mouse_pos = Vector(mouse_x, mouse_y);
    *velocity += (mouse_pos - fly_pos).normalized() * acceleration;

    if (velocity->x > max_velocity)
        velocity->x = max_velocity;
    if (velocity->x < -max_velocity)
        velocity->x = -max_velocity;
    if (velocity->y > max_velocity)
        velocity->y = max_velocity;
    if (velocity->y < -max_velocity)
        velocity->y = -max_velocity;

    return Vector(window_x + (int)round(velocity->x), window_y + (int)round(velocity->y));
}