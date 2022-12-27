#pragma once
#include "..\fly.h"

class FlyYellow: public Fly
{
protected:
    float acceleration = 1.0;
    float max_velocity = 15.0;
    Vector* velocity = nullptr;

public:
    Vector move(int window_x, int window_y, int mouse_x, int mouse_y);
};