#include "arcadia_sdl.h"

int arcadia_sdl_initialize_window(
    SDL_Window **window, 
    SDL_Renderer **renderer,
    int x,
    int y,
    int width,
    int height,
    char *title,
    Uint32 flags)
{
    LOG_SDL_REVISION_DEBUG();
    LOG_SDL_DEBUG("Initializing subsystems...");
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != EXIT_SUCCESS)
    {
        LOG_SDL_ERR("There was an error initializing the SDL library.");
        return false;
    }   
    
    LOG_SDL_DEBUG("Initializing window.");
    
    *window = SDL_CreateWindow(
        title,
        x,
        y,
        width,
        height,
        flags);

    if (*window == NULL)
    {
        LOG_SDL_ERR("Error creating SDL window.");
        return false;
    }

    LOG_SDL_DEBUG("Initializing renderer.");

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (*renderer == NULL)
    {
        LOG_SDL_ERR("Error creating renderer.");
        return false;
    }

    return true;
}


void arcadia_sdl_destroy_window(
    SDL_Window **window,
    SDL_Renderer **renderer)
{
    LOG_SDL_DEBUG("Destroying renderer...");
    SDL_DestroyRenderer(*renderer);

    LOG_SDL_DEBUG("Destroying window...");
    SDL_DestroyWindow(*window);

    LOG_SDL_DEBUG("Destroying subsystem...");
    SDL_Quit();
}

void arcadia_sdl_render_rectangle(
    SDL_Renderer *renderer, 
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float x4, float y4)
{
    SDL_FPoint points[] = 
    {
        (SDL_FPoint) { .x = x1, .y = y1},
        (SDL_FPoint) { .x = x2, .y = y2},
        (SDL_FPoint) { .x = x3, .y = y3},
        (SDL_FPoint) { .x = x4, .y = y4},
        (SDL_FPoint) { .x = x1, .y = y1}
    };

    SDL_RenderDrawLinesF(renderer, points, sizeof(points) / sizeof(SDL_FPoint));
}

void arcadia_sdl_render_triangle(
    SDL_Renderer *renderer,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3)
{
    SDL_FPoint points[] = 
    {
        (SDL_FPoint) { .x = x1, .y = y1},
        (SDL_FPoint) { .x = x2, .y = y2},
        (SDL_FPoint) { .x = x3, .y = y3},
        (SDL_FPoint) { .x = x1, .y = y1}
    };

    SDL_RenderDrawLinesF(renderer, points, sizeof(points) / sizeof(SDL_FPoint));
}

void arcadia_sdl_render_axis(
    SDL_Renderer *renderer, 
    int axis_pixels_width)
{
    int w = 0, h = 0;
    SDL_Window *window = SDL_RenderGetWindow(renderer);
    SDL_GetWindowSize(window, &w, &h);

    // x axis
    for (int i = -1 * (axis_pixels_width / 2); i < (axis_pixels_width / 2) + 1; i++)
    {
        SDL_RenderDrawLineF(
            renderer, 
            0.f,   h / 2.f + i,
            w + i, h / 2.f + i);
    }
    
    // y axis
    for (int i = -1 * (axis_pixels_width / 2); i < (axis_pixels_width / 2) + 1; i++)
    {
        SDL_RenderDrawLineF(
            renderer, 
            w / 2.f + i, 0,
            w / 2.f + i, h);
    }
}
