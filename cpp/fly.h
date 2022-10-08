#pragma once
#include "vector.h"

class Fly
{
private:
    int size = 64;
    float speed_chase = 5.0;
    float speed_idle = 1.0;
    Vector* target_point = nullptr;

public:
    Vector move(int window_x, int window_y, int mouse_x, int mouse_y);
};