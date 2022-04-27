#pragma once

#include <vector>

#include <SDL2/SDL.h>
#include <stdio.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <iostream>
#include <string>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE_MULTIPLIER 3

struct FrameRate
{
    float dT = 0;
    float elapsedMS = 0;
    Uint32 lastUpdate = 0;
    float timeToWait = 0;
    Uint32 time = 0;
    Uint64 start = 0;
    Uint64 end = 0;
};

class Camera
{
    float fov;

public:
    glm::mat4 projection;
    glm::vec4 view;

    Camera(float pFov = 60.0f) : fov(pFov)
    {
        projection = glm::perspective(glm::radians(fov), (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.f);
        view = glm::vec4(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
};

using namespace std;
class Program
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer;

    glm::mat4 cubeRotation;
    Camera camera;

    float cubeRotationAngle = 0.0f;

    FrameRate fr = {0};

    std::vector<glm::vec3> cube;

    bool isProgramRunning = true;

    void initCube(float sideSize, float step)
    {
        cube.reserve((int)ceil(sideSize * sideSize * sideSize));
        float halfSize = sideSize / 2;
        for (float x = -halfSize; x < halfSize; x += step)
            for (float y = -halfSize; y < halfSize; y += step)
                for (float z = -halfSize; z < halfSize; z += step)
                    cube.emplace_back((glm::vec3){x, y, z});
    }

    void updateDeltaTime()
    {
        fr.end = SDL_GetPerformanceCounter();
        fr.elapsedMS = (float)(fr.end - fr.start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        fr.timeToWait = floor(16.666f - fr.elapsedMS);
        SDL_Delay(fr.timeToWait >= 0 ? fr.timeToWait : 0);

        fr.start = SDL_GetPerformanceCounter();
        fr.time = SDL_GetTicks();
        fr.dT = (fr.time - fr.lastUpdate) / 1000.0f;
        fr.lastUpdate = fr.time;
    }

    void drawCube(float deltaTime)
    {
        cubeRotationAngle += 1.f * deltaTime;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        cubeRotation = glm::rotate(glm::mat4(1.0), cubeRotationAngle, glm::vec3(0, 1, 1));
        auto model = glm::translate(glm::mat4(1.0), glm::vec3(-0.25, -0.25, -50.f));
        int cubeSize = cube.size();
        for (int i = 0; i < cubeSize; i++)
        {
            auto vertexP = glm::project(cube[i], model * cubeRotation, camera.projection, camera.view);
            SDL_RenderDrawPoint(renderer, vertexP.x, vertexP.y);
        }
    }

    void handleEvents(void)
    {
        SDL_Event evt = {0};
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                isProgramRunning = false;
            }

            if (evt.type == SDL_KEYUP)
            {
                if (evt.key.keysym.sym == SDLK_ESCAPE)
                {
                    isProgramRunning = false;
                }

                if (evt.key.keysym.sym == SDLK_LEFT)
                {
                }
            }
        }
    }

    void clearScreen(int r = 0, int g = 0, int b = 0)
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);
    }

    void display(void)
    {
        SDL_RenderPresent(renderer);
    }

public:
    Program()
    {

        initCube(30.0f, 1.f);
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
            exit(-1);
        }
        window = SDL_CreateWindow("Drawing Pixels!!", 0, 0, SCREEN_WIDTH * SCREEN_SIZE_MULTIPLIER, SCREEN_HEIGHT * SCREEN_SIZE_MULTIPLIER, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);

        if (window == NULL)
        {
            fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
            exit(-1);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void runMainLoop()
    {
        while (isProgramRunning)
        {
            handleEvents();
            updateDeltaTime();
            clearScreen();
            drawCube(fr.dT);
            display();
        }
    }

    ~Program()
    {
        printf("Terminating\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};