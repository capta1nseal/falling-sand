#include "sandgrain.hpp"

#include <cmath>

SandGrain::SandGrain()
{
    exists = false;
    r = 0;
    g = 0;
    b = 0;
}
SandGrain::SandGrain(bool v)
{
    exists = v;
    r = 0;
    g = 0;
    b = 0;
}
SandGrain::SandGrain(int baseR, int baseG, int baseB)
    : r(baseR), g(baseG), b(baseB)
{
    exists = true;
}
SandGrain::SandGrain(double h, double s, double v)
{
    exists = true;

    h = fmodf64(h, 360.0) / 60;
    long i = static_cast<int>(h);
    double ff = h - i;
    double p = v * (1.0 - s);
    double q = v * (1.0 - (s * ff));
    double t = v * (1.0 - (s * (1.0 - ff)));

    switch (i)
    {
    case 0:
        r = 255 * v;
        g = 255 * t;
        b = 255 * p;
        break;
    case 1:
        r = 255 * q;
        g = 255 * v;
        b = 255 * p;
        break;
    case 2:
        r = 255 * p;
        g = 255 * v;
        b = 255 * t;
        break;
    case 3:
        r = 255 * p;
        g = 255 * q;
        b = 255 * v;
        break;
    case 4:
        r = 255 * t;
        g = 255 * p;
        b = 255 * v;
        break;
    case 5:
    default:
        r = 255 * v;
        g = 255 * p;
        b = 255 * q;
        break;
    }
}

void SandGrain::operator=(const bool &other)
{
    exists = other;
}

bool SandGrain::operator==(bool other)
{
    return exists == other;
}
