#include "fly_red.h"
#include <random>

Vector FlyRed::move(int window_x, int window_y, int mouse_x, int mouse_y)
{
    Vector fly_pos = Vector(window_x + size/2, window_y + size/2);
    Vector mouse_pos = Vector(mouse_x, mouse_y);

    if (fly_pos.distance_to(mouse_pos) > 160.0)
    {
        target_point = nullptr;
        Vector travel = ((mouse_pos - fly_pos).normalized() * speed_chase);
        return Vector(window_x + (int)travel.x, window_y + (int)travel.y);
    }
    else
    {
        if (target_point == nullptr)
        {
            int rand_x = (rand() % 281) - 140;
            int rand_y = (rand() % 281) - 140;
            target_point = new Vector(mouse_x + rand_x, mouse_y + rand_y);
        }

        if (fly_pos.distance_to(*target_point) < 5.0)
        {
            int rand_x = (rand() % 281) - 140;
            int rand_y = (rand() % 281) - 140;
            target_point = new Vector(mouse_x + rand_x, mouse_y + rand_y);
        }
        
        Vector travel = ((*target_point - fly_pos).normalized() * speed_idle);
        return Vector(window_x + (int)round(travel.x), window_y + (int)round(travel.y));
    }
}