#ifndef ARCADIA_SDL_H
#define ARCADIA_SDL_H

#include "SDL.h"
#include "stdbool.h"
#include "stdio.h"

#define LOG_SDL_ERR(x) fprintf(stderr, "[SDL] " #x " Error: %s \n", SDL_GetError());
#define LOG_SDL_DEBUG(x) fprintf(stdout, "[SDL] " #x "\n");  
#define LOG_SDL_REVISION_DEBUG() fprintf(stdout, "[SDL] SDL Revision: %s\n", SDL_GetRevision());  

int arcadia_sdl_initialize_window(
    SDL_Window **window, 
    SDL_Renderer **renderer,
    int x,
    int y,
    int width,
    int height,
    char *title,
    Uint32 flags);

void arcadia_sdl_destroy_window(
    SDL_Window **window,
    SDL_Renderer **renderer);

void arcadia_sdl_render_rectangle(
    SDL_Renderer *renderer, 
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float x4, float y4);

void arcadia_sdl_render_triangle(
    SDL_Renderer *renderer,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3
);

void arcadia_sdl_render_axis(
    SDL_Renderer *renderer, 
    int axis_pixels_width);

#endif