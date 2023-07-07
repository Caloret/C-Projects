#include "arcadia_sdl.h"
#include "SDL.h"
#include "SDL_version.h"
#include "SDL_image.h"
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
SDL_Texture *grass_texture = NULL;

int window_width = WIDTH;
int window_height = HEIGHT;

int last_frame_time = 0;
int frame_number = 0;

// Objects
char *grass_image_path = "textures/grass.png";
SDL_Rect moving_grass;


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
    
    double factor = 50;

    // reset frame number
    frame_number = frame_number > (factor * FPS) ? 0 : frame_number;
    frame_number++;
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, ARCADIA_COL_BLACK);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, grass_texture, NULL, &moving_grass);

    SDL_RenderPresent(renderer);
}

