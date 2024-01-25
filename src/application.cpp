#include "application.hpp"

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

#include <SDL2/SDL.h>

#include "simulation.hpp"

std::chrono::_V2::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

FallingSandApplication::FallingSandApplication()
{
    initializeSdl();
    initializeSimulation();

    isRunning = false;
    isFullscreen = false;
}

void FallingSandApplication::run()
{
    auto start = now();

    std::chrono::duration<double> delta;

    int frameCounter = 0;

    simulationThread = std::thread(&FallingSandSimulation::simulationLoop, &fallingSandSimulation);

    isRunning = true;
    draw();

    while (isRunning)
    {
        start = now();
        
        handleEvents();

        draw();

        delta = now() - start;

        if (frameCounter % static_cast<int>(10.0 / (delta.count() + 0.0001)) == 0)
        {
            std::cout << delta.count() << "\n";
        }

        frameCounter++;
    }

    fallingSandSimulation.stop();

    simulationThread.join();

    destroySdl();
}

void FallingSandApplication::initializeSdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GetCurrentDisplayMode(0, &displayMode);

    displayWidth = displayMode.w / 2;
    displayHeight = displayMode.h / 2;

    uint32_t windowFlags = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow(
        "falling sand",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        displayWidth, displayHeight,
        windowFlags);
    
    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}
void FallingSandApplication::destroySdl()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void FallingSandApplication::initializeSimulation()
{
    fallingSandSimulation.initializeSimulation(displayWidth, displayHeight);
}

void FallingSandApplication::handleEvents()
{
    SDL_PumpEvents();
    
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                displayWidth = event.window.data1;
                displayHeight = event.window.data2;
                
                fallingSandSimulation.initializeSimulation(displayWidth, displayHeight);
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_F11:
                if (isFullscreen)
                {
                    SDL_SetWindowFullscreen(window, 0);
                }
                else
                {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                isFullscreen = !isFullscreen;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                fallingSandSimulation.spawn(event.button.x, event.button.y, 50);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void FallingSandApplication::draw()
{
    std::lock_guard<std::mutex> lock(renderMutex);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    { // draw the sand grid
        const std::vector<bool>& sandGrid = fallingSandSimulation.getSandGrid();

        SDL_SetRenderDrawColor(renderer, 255, 127, 31, 255);

        for (unsigned int x = 0; x < displayWidth; x++)
        {
            for (unsigned int y = 0; y < displayHeight; y++)
            {
                if (sandGrid[x * displayHeight + y])
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}