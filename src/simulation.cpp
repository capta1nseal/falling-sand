#include "simulation.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

FallingSandSimulation::FallingSandSimulation()
{
    m_width = 1;
    m_height = 1;

    sandGrid1.resize(1);
    sandGrid2.resize(1);

    newSandGrid = *(&sandGrid1);
    oldSandGrid = *(&sandGrid2);
}

void FallingSandSimulation::initializeSimulation(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;

    sandGrid1.resize(width * height); // ensure enough memory is available in the vector
    sandGrid1.assign(width * height, false); // assign all necessary values to false

    sandGrid2.resize(width * height);
    sandGrid2.assign(width * height, false);

    newSandGrid = *(&sandGrid1);
    oldSandGrid = *(&sandGrid2);
}

void FallingSandSimulation::tick()
{
    swapGrids();

    oldAt(rand() % 100, rand() % 100) = true;

    newSandGrid = oldSandGrid;

    for (unsigned int x = 0; x < m_width; x++)
    {
        for (unsigned int y = 0; y < m_height; y++)
        {
            if (oldAt(x, y))
            {
                if (not oldAt(x, y + 1))
                {
                    std::swap(at(x, y), at(x, y + 1));
                    continue;
                }
            }
        }
    }
}

void FallingSandSimulation::swapGrids()
{
    std::swap(newSandGrid, oldSandGrid);
}

void FallingSandSimulation::draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 127, 31, 255);

    for (unsigned int x = 0; x < m_width; x++)
    {
        for (unsigned int y = 0; y < m_height; y++)
        {
            if (at(x, y))
            {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

std::_Bit_reference FallingSandSimulation::at(unsigned int x, unsigned int y)
{
    if (y == m_height)
    {
        return newSandGrid[x * m_height + y - 1];
    }

    return newSandGrid[x * m_height + y];
}

std::_Bit_reference FallingSandSimulation::oldAt(unsigned int x, unsigned int y)
{
    if (y == m_height)
    {
        return oldSandGrid[x * m_height + y - 1];
    }

    return oldSandGrid[x * m_height + y];
}
