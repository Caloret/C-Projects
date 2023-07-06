#include "arcadia_sdl.h"
#include "SDL.h"
#include "SDL_version.h"
#include "stdbool.h"
#include "constants.h"

#define WINDOW_TITLE "Hello Window!"
#define POSITION_X SDL_WINDOWPOS_CENTERED
#define POSITION_Y SDL_WINDOWPOS_CENTERED
#define WIDTH 600
#define HEIGHT 500
#define MAX_NUMBER_OF_POINTS (WIDTH * HEIGHT)
#define NUMBER_OF_POINTS_PER_PLOT (WIDTH)
#define PIXELS_PER_UNIT (WIDTH / 5)

int app_is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;
SDL_Surface *optimized_screen_surface = NULL;

int window_width = WIDTH;
int window_height = HEIGHT;

int last_frame_time = 0;
int frame_number = 0;

// Objects
char *grass_image_path = "textures/forest.bmp";
SDL_Surface *grass_image = NULL;

bool setup(void);

void process_input(void);

void update(void);

void render(void);

int main(int argc, char *argv[])
{
    // To avoid warnings of unused parameters
    argc = argc;
    argv = argv;

    app_is_running = arcadia_sdl_initialize_window(
        &window, 
        &renderer, 
        POSITION_X,
        POSITION_Y,
        WIDTH,
        HEIGHT,
        WINDOW_TITLE,
        SDL_WINDOW_RESIZABLE);
    
    app_is_running = app_is_running && setup();

    // Render loop
    while (app_is_running)
    {
        process_input();
        update();
        render();
    }

    // Free Resources
    SDL_FreeSurface(screen_surface);
    SDL_FreeSurface(optimized_screen_surface);

    // Free Window and subsystems
    arcadia_sdl_destroy_window(&window, &renderer);
    
    return EXIT_SUCCESS;
}


bool setup(void)
{
    screen_surface = SDL_GetWindowSurface(window);

    grass_image = SDL_LoadBMP(grass_image_path);
    if (grass_image == NULL)
    {
        LOG_SDL_DEBUG("Unable to Load image.");
        return false;
    }

    optimized_screen_surface = SDL_ConvertSurface(grass_image, screen_surface->format, 0);
    if (optimized_screen_surface == NULL)
    {
        LOG_SDL_DEBUG("Unable to optimize surface.");
        return false;
    }

    SDL_FreeSurface(grass_image);
    
    return true;
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                SDL_GetWindowSize(window, &window_width, &window_height);
                screen_surface = SDL_GetWindowSurface(window);
            }
            break;
        case SDL_QUIT:
            app_is_running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                app_is_running = false;
            break;
    }

    return;
}

void update(void)
{
    int time_to_wait = FRAME_TARGET_TIME_MS - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME_MS)
    {
        SDL_Delay(time_to_wait);
    }
    
    double factor = 50;

    // reset frame number
    frame_number = frame_number > (factor * FPS) ? 0 : frame_number;
    frame_number++;
}

void render(void)
{
    SDL_Rect strectchRect = {
        .x = 0,
        .y = 0,
        .w = window_width,
        .h = window_height
    };

    SDL_BlitScaled(optimized_screen_surface, NULL, screen_surface, &strectchRect);
    
    SDL_UpdateWindowSurface(window);
}

