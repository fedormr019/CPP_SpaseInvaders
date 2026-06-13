#pragma once
#include <cmath>

class Vec2 {
public:
    double x, y;

    Vec2(double x = 0, double y = 0);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(double scalar) const;
    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(double scalar);

    double length() const;
    void normalize();
};