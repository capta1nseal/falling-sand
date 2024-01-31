#include "simulation.hpp"

#include <vector>
#include <algorithm>
#include <mutex>
#include <random>

#include <SDL2/SDL_rect.h>

FallingSandSimulation::FallingSandSimulation()
{
    m_width = 1;
    m_height = 1;

    sandGrid1.resize(1);
    sandGrid2.resize(1);

    safe_sandGrid.resize(1);

    newGridNumber = 1;
    newSandGrid = *(&sandGrid1);
    oldSandGrid = *(&sandGrid2);

    coinFlip = std::uniform_int_distribution<int>(0, 1);

    isRunning = false;

    droppingSand = false;
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

    safe_sandGrid.resize(width * height);
    safe_sandGrid = sandGrid1;

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

void FallingSandSimulation::setMousePosition(SDL_Point *mousePosition)
{
    m_mousePosition = mousePosition;
}

void FallingSandSimulation::toggleDroppingSand()
{
    droppingSand = !droppingSand;

    if (!droppingSand) return;

    using UniformDistribution = std::uniform_real_distribution<double>;

    droppingHue = (UniformDistribution(0.0, 360.0))(randomEngine);
    droppingSaturation = (UniformDistribution(0.25, 0.75))(randomEngine);
    droppingValue = (UniformDistribution(0.625, 0.875))(randomEngine);
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

void FallingSandSimulation::getFrameData(std::vector<SandGrain> &sandGrid)
{
    std::lock_guard<std::mutex> lock(simulationMutex);

    sandGrid.resize(m_width * m_height);

    sandGrid = safe_sandGrid;
}

void FallingSandSimulation::spawn(unsigned int x, unsigned int y, unsigned int radius)
{
    using UniformDistribution = std::uniform_real_distribution<double>;

    double baseH = (UniformDistribution(0.0, 360.0))(randomEngine);
    double baseS = (UniformDistribution(0.25, 0.75))(randomEngine);
    double baseV = (UniformDistribution(0.625, 0.875))(randomEngine);

    spawn(x, y, radius, baseH, baseS, baseV);
}
void FallingSandSimulation::spawn(unsigned int x, unsigned int y, unsigned int radius, double hue, double saturation, double value)
{
    if (radius > m_width / 2) radius = m_width / 2;
    if (radius > m_height / 2) radius = m_height / 2;
    if (x < radius) x = radius;
    if (x > m_width - 1 - radius) x = m_width - 1 - radius;
    if (y < radius) y = radius;
    if (y > m_height - 1 - radius) y = m_height - 1 - radius;

    using UniformDistribution = std::uniform_real_distribution<double>;

    UniformDistribution saturationDistribution(-0.05, 0.05);
    UniformDistribution valueDistribution(-0.05, 0.05);

    std::lock_guard<std::mutex> lock(simulationMutex);
    for (unsigned int i = x - radius; i < x + radius - 1; i++)
    {
        for (unsigned int j = y - radius; j < y + radius - 1; j++)
        {
            oldAt(i, j) = SandGrain(
                hue,
                std::min(1.0, std::max(0.0, saturation + saturationDistribution(randomEngine))),
                std::min(1.0, std::max(0.0, value + valueDistribution(randomEngine)))
            );
            at(i, j) = oldAt(i, j);
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
            
            bool current = at(x, y) == true;

            if (current)
            {
                if (y == m_height - 1) continue;
                bool under = at(x, y + 1) == true;

                if (not under)
                {
                    std::swap(oldAt(x, y), oldAt(x, y + 1));
                    continue;
                }

                bool left, right;

                if (x < 1) left = true;
                else left = at(x - 1, y) == true;

                if (x > m_width - 1) right = true;
                else right = at(x + 1, y) == true;

                if (left and right) continue;

                bool downRight = at(x + 1, y + 1) == true;
                bool downLeft = at(std::max(0, static_cast<int>(x - 1)), y + 1) == true;

                if (not (downLeft or downRight))
                {
                    if (not (left or right))
                    {
                        std::swap(oldAt(x, y), oldAt(x + (1 - 2 * (coinFlip(randomEngine))), y + 1));
                        continue;
                    }
                    if (left) std::swap(oldAt(x, y), oldAt(x + 1, y + 1));
                    else std::swap(oldAt(x, y), oldAt(x - 1, y + 1));
                    continue;
                }
                if (downRight and not downLeft and not left)
                {
                    std::swap(oldAt(x, y), oldAt(std::max(0, static_cast<int>(x - 1)), y + 1));
                    continue;
                }
                if (downLeft and not downRight and not right)
                {
                    std::swap(oldAt(x, y), oldAt(x + 1, y + 1));
                }
            }
        }
    }

    if (droppingSand)
    {
        spawn(m_mousePosition->x, m_mousePosition->y, 6, droppingHue, droppingSaturation, droppingValue);
        droppingHue += 0.01;
    }

    swapGrids();

    {
        std::lock_guard<std::mutex> lock(simulationMutex);

        safe_sandGrid = newSandGrid;
    }
}

SandGrain& FallingSandSimulation::at(unsigned int x, unsigned int y)
{
    if (y == m_height) return at(x, y - 1);

    if (x > m_width - 1) return at(m_width - 1, y);

    return newSandGrid[x * m_height + y];
}

SandGrain& FallingSandSimulation::oldAt(unsigned int x, unsigned int y)
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
