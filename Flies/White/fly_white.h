#pragma once
#include "..\fly.h"

class FlyWhite: public Fly
{
protected:
    float distance = 60.0;
    float speed = 0.02;
    float rotation = 0.0;

public:
    Vector move(int window_x, int window_y, int mouse_x, int mouse_y);
};