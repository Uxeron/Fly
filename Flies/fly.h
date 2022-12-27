#pragma once
#include "..\vector.h"

class Fly
{
protected:
    int size = 64;

public:
    virtual Vector move(int window_x, int window_y, int mouse_x, int mouse_y);
};