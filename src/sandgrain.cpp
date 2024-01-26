#include "sandgrain.hpp"

SandGrain::SandGrain()
{
    exists = false;
    r = 0;
    g = 0;
    b = 0;
}
SandGrain::SandGrain(bool value)
{
    exists = value;
    r = 0;
    g = 0;
    b = 0;
}
SandGrain::SandGrain(int baseR, int baseG, int baseB)
    : r(baseR), g(baseG), b(baseB)
{
    exists = true;
}

void SandGrain::operator=(const bool &other)
{
    exists = other;
}

bool SandGrain::operator==(bool other)
{
    return exists == other;
}
