#ifndef ARCADIA_SDL_H
#define ARCADIA_SDL_H

#include "SDL.h"
#include "stdbool.h"
#include "stdio.h"

#define LOG_SDL_ERR(x) fprintf(stderr, "[SDL] " #x " Error: %s \n", SDL_GetError());
#define LOG_SDL_DEBUG(x) fprintf(stdout, "[SDL] " #x "\n");  
#define LOG_SDL_REVISION_DEBUG() fprintf(stdout, "[SDL] SDL Revision: %s\n", SDL_GetRevision());  


// Colors Definition for SDL
#define ARCADIA_COL_BLACK 0,0,0,255
#define ARCADIA_COL_WHITE 255,255,255,255


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
    size_t axis_border_width_pixels,
    size_t pixels_per_unit, 
    size_t number_of_units);

void arcadia_sdl_translate_point_to_axis(
    const float x_in,
    const float y_in,
    const float w,
    const float h,
    float *x_out,
    float *y_out);

#endif