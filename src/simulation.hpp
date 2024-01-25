#ifndef _FALLINGSANDSIMULATION
#define _FALLINGSANDSIMULATION

#include <vector>
#include <SDL2/SDL_render.h>

class FallingSandSimulation
{
public:
    FallingSandSimulation();

    void initializeSimulation(unsigned int width, unsigned int height);

    void tick();

    void draw(SDL_Renderer *renderer);

private:
    unsigned int m_width, m_height;
    std::vector<bool> sandGrid1;
    std::vector<bool> sandGrid2;
    std::vector<bool> newSandGrid;
    std::vector<bool> oldSandGrid;

    std::_Bit_reference at(unsigned int x, unsigned int y);
    std::_Bit_reference oldAt(unsigned int x, unsigned int y);

    void swapGrids();

};

#endif
