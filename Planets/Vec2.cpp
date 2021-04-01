#include "Vec2.h"
#include <cmath>
#include <utility>

Vec2::Vec2()
{
    Vec2(0, 0);
}

Vec2::Vec2(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Vec2::dot(Vec2& v)
{
    return (this->x * v.x) + (this->y * v.y);
}

double Vec2::magn()
{
    return sqrt((this->x * this->x) + (this->y * this->y));
}

double Vec2::radAngle(Vec2& v)
{
    return acos(this->dot(v) / (this->magn() * v.magn()));
}

Vec2 Vec2::unit()
{
    return Vec2(this->x / this->magn(), this->y / this->magn());
}

Vec2& Vec2::operator=(const Vec2& v)
{
    this->x = v.x;
    this->y = v.y;

    return *this;
}

bool operator==(const Vec2& a, const Vec2& b)
{
    return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const Vec2& a, const Vec2& b)
{
    return (a.x != b.x) || (a.y != b.y);
}

Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x + b.x, a.y + b.y);
}

void operator+=(Vec2& a, const Vec2& b)
{
    a = a + b;
}

Vec2 operator-(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x - b.x, a.y - b.y);
}

void operator-=(Vec2& a, const Vec2& b)
{
    a = a - b;
}

Vec2 operator*(const Vec2& v, const double k)
{
    return Vec2(k * v.x, k * v.y);
}

Vec2 operator*(const double k, const Vec2& v)
{
    return v * k;
}

void operator*=(Vec2& v, const double k)
{
    v = v * k;
}

Vec2 operator/(const Vec2& v, const double k)
{
    return v * (1 / k);
}

void operator/=(Vec2& v, const double k)
{
    v = v / k;
}

std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
    out << "[" << v.x << ", " << v.y << "]";

    return out;
}
