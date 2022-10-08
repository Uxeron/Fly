#include "vector.h"
#include <cmath>

Vector::Vector (float _x, float _y)
{
    x = _x;
    y = _y;
}

float Vector::length()
{
    return sqrt(pow(x, 2.0) + pow(y, 2.0));
}

Vector Vector::normalized()
{
    if (length() == 0.0)
        return Vector(0.0, 0.0); 
    return Vector(x / length(), y / length());
}

float Vector::distance_to(Vector const &other)
{
    return sqrt(pow(other.x - x, 2.0) + pow(other.y - y, 2.0));
}

Vector Vector::operator+(Vector const &other)
{
    return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(Vector const &other)
{
    return Vector(x - other.x, y - other.y);
}

Vector Vector::operator*(float mul)
{
    return Vector(x * mul, y * mul);
}