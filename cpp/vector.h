#pragma once

class Vector
{
public:
    float x = 0.0;
    float y = 0.0;

    Vector (float _x, float _y);

    float length();
    Vector normalized();
    float distance_to(Vector const &other);

    Vector operator+(Vector const &other);
    Vector operator-(Vector const &other);
    Vector operator*(float mul);
};