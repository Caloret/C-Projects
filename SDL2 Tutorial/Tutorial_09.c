#include "stdint.h"
#include "arcadia_sdl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_image.h>
#include "stdbool.h"
#include "constants.h"
#include <stdlib.h>

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
SDL_Texture *grass_texture = NULL;

int window_width = WIDTH;
int window_height = HEIGHT;

int last_frame_time = 0;
int frame_number = 0;

// Objects
char *grass_image_path = "textures/grass.png";
SDL_Rect moving_grass;
SDL_Vertex vertices[] = {
    { { 0,        		 0 },    {ARCADIA_COL_WHITE}, {0, 1}},
    { { WIDTH,    		 0 },    {ARCADIA_COL_WHITE}, {1, 0}},
    { { WIDTH/2,  HEIGHT/2 },    {ARCADIA_COL_WHITE}, {1, 1}},
    { { WIDTH,    		 0 }, 	 {ARCADIA_COL_WHITE}, {0, 1}},
    { { WIDTH,      HEIGHT },    {ARCADIA_COL_WHITE}, {1, 0}},
    { { WIDTH/2,  HEIGHT/2 },    {ARCADIA_COL_WHITE}, {1, 1}},
    { { WIDTH/2,  HEIGHT/2 },    {ARCADIA_COL_WHITE}, {0, 1}},
    { { WIDTH,      HEIGHT },    {ARCADIA_COL_WHITE}, {1, 0}},
    { { 0,          HEIGHT },    {ARCADIA_COL_WHITE}, {1, 1}},
    { { WIDTH/2,  HEIGHT/2 },    {ARCADIA_COL_WHITE}, {0, 1}},
    { { 0,          HEIGHT },    {ARCADIA_COL_WHITE}, {1, 0}},
    { { 0,                0},    {ARCADIA_COL_WHITE}, {1, 1}},

};

SDL_FPoint circle_points[WIDTH * HEIGHT] = {0};

bool setup(void);

void process_input(void);

void update(void);

void render(void);

int random_int(int min, int max);

// SDL2 forces to define main function with argc and argv arguments:w
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
    SDL_DestroyTexture(grass_texture);
    
    // Free Window and subsystems
    arcadia_sdl_destroy_window(&window, &renderer);
    
    return EXIT_SUCCESS;
}


bool setup(void)
{
    moving_grass = (SDL_Rect) {
        .x = 0,
        .y = 0,
        .w = 100,
        .h = 100    
    };

    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags))
    {
        LOG_SDL_DEBUG("Error initializing image library.");
        return false;
    }

    grass_texture = IMG_LoadTexture(renderer, grass_image_path);
    if (grass_texture == NULL)
    {
        LOG_SDL_DEBUG("Unable to Load image.");
        return false;
    }

	LOG_SDL_DEBUG("Grass Texture loaded.");
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
            }
            break;
        case SDL_QUIT:
            app_is_running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                app_is_running = false;
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                if (moving_grass.y <= window_height - moving_grass.h)
                    moving_grass.y += 20;
            }
            else if (event.key.keysym.sym == SDLK_RIGHT)
            {
                if (moving_grass.x <= window_width - moving_grass.w)
                    moving_grass.x += 20;
            }   
            
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
    
    // reset frame number
    frame_number = frame_number > FPS ? 0 : frame_number;
    frame_number++;
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, ARCADIA_COL_BLACK);
    SDL_RenderClear(renderer);

    for (size_t group_circles = 0; group_circles < 4; ++group_circles)
    {
        for (int num_circles = 0; num_circles < 10; ++num_circles)
        {
            SDL_SetRenderDrawColor(renderer, ARCADIA_COL_WHITE);
            arcadia_sdl_render_circle(
                renderer,
                group_circles * window_width/8  + random_int(50, 200), 
                group_circles * window_height/8 + random_int(50, 200),
                random_int(5, 10),
                window_width,
                window_height);
                   
            SDL_SetRenderDrawColor(renderer, ARCADIA_COL_RED);
            arcadia_sdl_render_filled_circle(
                renderer,
                group_circles * window_width/8  - random_int(50, 200),
                group_circles * window_height/8 - random_int(50, 200),
                random_int(5, 10),
                window_width,
                window_height);
        }
    } 

    SDL_RenderPresent(renderer);
	
}


int random_int(int min, int max)
{
    return min + rand() % (max + 1 - min);
}
