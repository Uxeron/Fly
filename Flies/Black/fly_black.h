#pragma once
#include "..\fly.h"

class FlyBlack: public Fly
{
protected:
    float speed_chase = 3.0;
    float speed_idle = 1.0;
    Vector* target_point = nullptr;

public:
    Vector move(int window_x, int window_y, int mouse_x, int mouse_y);
};