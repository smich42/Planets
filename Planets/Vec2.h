#pragma once
#include <ostream>

class Vec2
{
public:
    double x, y;

    Vec2();
    Vec2(double x, double y);

    double dot(Vec2& v);
    double magn();

    double radAngle(Vec2& v);

    Vec2 unit();

    Vec2& operator=(const Vec2&);
};

bool operator==(const Vec2&, const Vec2&);

bool operator!=(const Vec2&, const Vec2&);

Vec2 operator+(const Vec2&, const Vec2&);

void operator+=(Vec2&, const Vec2&);

Vec2 operator-(const Vec2&, const Vec2&);

void operator-=(Vec2&, const Vec2&);

Vec2 operator*(const Vec2&, const double);

Vec2 operator*(const double, const Vec2&);

void operator*=(Vec2&, const double);

Vec2 operator/(const Vec2&, const double);

void operator/=(Vec2&, const double);

std::ostream& operator<<(std::ostream&, const Vec2&);
