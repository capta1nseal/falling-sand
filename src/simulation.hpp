#ifndef _FALLINGSANDSIMULATION
#define _FALLINGSANDSIMULATION

class FallingSandSimulation
{
public:
    FallingSandSimulation();

    void initializeSimulation(unsigned int width, unsigned int height);

    void tick();

private:
    unsigned int m_width, m_height;

};

#endif
