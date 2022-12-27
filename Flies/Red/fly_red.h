#pragma once
#include "..\fly.h"

class FlyRed: public Fly
{
protected:
    float speed_chase = 10.0;
    float speed_idle = 3.0;
    Vector* target_point = nullptr;

public:
    Vector move(int window_x, int window_y, int mouse_x, int mouse_y);
};