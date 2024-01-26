#ifndef _FALLINGSANDSIMULATION
#define _FALLINGSANDSIMULATION

#include <vector>
#include <mutex>

#include "sandgrain.hpp"

class FallingSandSimulation
{
public:
    FallingSandSimulation();

    void initializeSimulation(unsigned int width, unsigned int height);

    void stop();

    void simulationLoop();

    const std::vector<SandGrain>& getSandGrid();

    void spawn(unsigned int x, unsigned int y, unsigned int radius);

private:
    unsigned int m_width, m_height;
    std::vector<SandGrain> sandGrid1;
    std::vector<SandGrain> sandGrid2;
    int newGridNumber;
    std::vector<SandGrain> newSandGrid;
    std::vector<SandGrain> oldSandGrid;

    bool isRunning;
    std::mutex simulationMutex;

    void tick();

    SandGrain& at(unsigned int x, unsigned int y);
    SandGrain& oldAt(unsigned int x, unsigned int y);

    void swapGrids();
};

#endif
