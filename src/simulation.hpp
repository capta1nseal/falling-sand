#ifndef _FALLINGSANDSIMULATION
#define _FALLINGSANDSIMULATION

#include <vector>
#include <mutex>

class FallingSandSimulation
{
public:
    FallingSandSimulation();

    void initializeSimulation(unsigned int width, unsigned int height);

    void stop();

    void simulationLoop();

    const std::vector<bool>& getSandGrid();

    void spawn(unsigned int x, unsigned int y, unsigned int radius);

private:
    unsigned int m_width, m_height;
    std::vector<bool> sandGrid1;
    std::vector<bool> sandGrid2;
    int newGridNumber;
    std::vector<bool> newSandGrid;
    std::vector<bool> oldSandGrid;

    bool isRunning;
    std::mutex simulationMutex;

    void tick();

    std::_Bit_reference at(unsigned int x, unsigned int y);
    std::_Bit_reference oldAt(unsigned int x, unsigned int y);

    void swapGrids();
};

#endif
