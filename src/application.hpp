#ifndef _FALLINGSANDAPPLICATION
#define _FALLINGSANDAPPLICATION

#include <mutex>
#include <thread>

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

    SDL_Texture *renderTexture;
    unsigned char* texturePixels;
    int texturePitch;

    SDL_Event event;
    
    SDL_Point mousePosition;
    SDL_Point mappedMousePosition;

    unsigned int simulationWidth, simulationHeight;
    FallingSandSimulation fallingSandSimulation;

    std::thread simulationThread;

    std::mutex renderMutex;

    void initializeSdl();
    void destroySdl();

    void initializeSimulation();

    void initializeRenderTexture();

    void handleEvents();

    void draw();

};

#endif
