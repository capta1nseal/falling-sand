#ifndef _FALLINGSANDAPPLICATION
#define _FALLINGSANDAPPLICATION

#include <SDL2/SDL.h>

#include "simulation.hpp"

class FallingSandApplication
{
public:
    FallingSandApplication();

    void run();

private:
    bool isRunning;
    unsigned int displayWidth, displayHeight;
    bool isFullscreen;
    
    SDL_DisplayMode displayMode;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Event event;
    
    SDL_Point mousePosition;

    FallingSandSimulation fallingSandSimulation;

    void initializeSdl();
    void destroySdl();

    void initializeSimulation();

    void handleEvents();

    void tick();

    void draw();

};

#endif
