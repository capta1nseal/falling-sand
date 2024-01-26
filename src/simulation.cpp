#include "simulation.hpp"

#include <vector>
#include <algorithm>
#include <mutex>

FallingSandSimulation::FallingSandSimulation()
{
    m_width = 1;
    m_height = 1;

    sandGrid1.resize(1);
    sandGrid2.resize(1);

    newGridNumber = 1;
    newSandGrid = *(&sandGrid1);
    oldSandGrid = *(&sandGrid2);

    isRunning = false;
}

void FallingSandSimulation::initializeSimulation(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;

    // acquire and lock mutex in case resizing is done at a bad time
    std::lock_guard<std::mutex> lock(simulationMutex);

    // ensure enough memory is available in the vectors
    // then assign false to all values
    sandGrid1.resize(width * height); 
    sandGrid1.assign(width * height, false);

    sandGrid2.resize(width * height);
    sandGrid2.assign(width * height, false);

    newGridNumber = 1;
    newSandGrid = *(&sandGrid1);
    oldSandGrid = *(&sandGrid2);

    for (int y = 0; y < std::min(10, static_cast<int>(height - 1)); y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            at(x, y) = true;
            oldAt(x, y) = true;
        }
    }
}

void FallingSandSimulation::stop()
{
    isRunning = false;
}

void FallingSandSimulation::simulationLoop()
{
    isRunning = true;
    while (isRunning)
    {
        tick();
    }
}

const std::vector<bool>& FallingSandSimulation::getSandGrid()
{
    std::lock_guard<std::mutex> lock(simulationMutex);
    return newSandGrid;
}

void FallingSandSimulation::spawn(unsigned int x, unsigned int y, unsigned int radius)
{
    if (radius > m_width / 2 - 1) radius = m_width / 2 - 1;
    if (radius > m_height / 2 - 1) radius = m_height / 2 - 1;
    if (x < radius) x = radius;
    if (x > m_width - 1 - radius) x = m_width - 1 - radius;
    if (y < radius) y = radius;
    if (y > m_height - 1 - radius) y = m_height - 1 - radius;

    for (unsigned int i = x - radius; i < x + radius - 1; i++)
    {
        for (unsigned int j = y - radius; j < y + radius - 1; j++)
        {
            at(i, j) = true;
            oldAt(i, j) = true;
        }
    }
}

void FallingSandSimulation::tick()
{
    oldSandGrid = newSandGrid;

    for (unsigned int x = 0; x < m_width; x++)
    {
        std::lock_guard<std::mutex> lock(simulationMutex);
        for (unsigned int y = 0; y < m_height; y++)
        {
            
            bool current = at(x, y);
            bool under = at(x, y + 1);

            if (current)
            {
                if (not under)
                {
                    std::swap(oldAt(x, y), oldAt(x, y + 1));
                    continue;
                }

                bool left = at(std::max(static_cast<int>(x - 1), 0), y);
                bool right = at(x + 1, y);

                if (left and right) continue;

                bool downRight = at(x + 1, y + 1);
                bool downLeft = at(std::max(0, static_cast<int>(x - 1)), y + 1);

                if (not (downLeft or downRight))
                {
                    std::swap(oldAt(x, y), oldAt(x + (1 - 2 * (rand() % 2)), y + 1));
                    continue;
                }
                if (downRight)
                {
                    std::swap(oldAt(x, y), oldAt(std::max(0, static_cast<int>(x - 1)), y + 1));
                    continue;
                }
                std::swap(oldAt(x, y), oldAt(x + 1, y + 1));
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(simulationMutex);

        swapGrids();
    }
}

std::_Bit_reference FallingSandSimulation::at(unsigned int x, unsigned int y)
{
    if (y == m_height) return at(x, y - 1);

    if (x > m_width - 1) return at(m_width - 1, y);

    return newSandGrid[x * m_height + y];
}

std::_Bit_reference FallingSandSimulation::oldAt(unsigned int x, unsigned int y)
{
    if (y == m_height) return oldAt(x, y - 1);

    if (x > m_width - 1) return oldAt(m_width - 1, y);

    return oldSandGrid[x * m_height + y];
}

void FallingSandSimulation::swapGrids()
{
    std::swap(newSandGrid, oldSandGrid);
    newGridNumber = 3 - newGridNumber;
}
