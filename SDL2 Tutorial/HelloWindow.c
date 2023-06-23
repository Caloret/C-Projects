#include "stdio.h"
#include "SDL.h"
#include "SDL_version.h"
#include "stdbool.h"
#include "constants.h"
#include "math.h"

#define WINDOW_TITLE "Hello Window!"
#define POSITION_X SDL_WINDOWPOS_CENTERED
#define POSITION_Y SDL_WINDOWPOS_CENTERED
#define WIDTH 600
#define HEIGHT 500

#define LOG_SDL_ERR(x) fprintf(stderr, "[SDL] " #x " Error: %s \n", SDL_GetError());
#define LOG_SDL_DEBUG(x) fprintf(stdout, "[SDL] " #x "\n");  
#define LOG_SDL_REVISION_DEBUG() fprintf(stdout, "[SDL] SDL Revision: %s\n", SDL_GetRevision());  

int app_is_running = false;
SDL_Window *window;
SDL_Renderer *renderer;
int last_frame_time = 0;
int frame_number = 0;

double background_colour_R = 0.0;
double background_colour_G = 0.0;
double background_colour_B = 0.0;
double background_colour_A = 0.0;

// Objects
SDL_Rect ball;
float ball_speed_x_axis = 200.0f;
float ball_speed_y_axis = 200.0f;

int initialize_window(void);

void setup(void);

void destroy_window(void);

void process_input(void);

void update(void);

void render(void);

int main(int argc, char *argv[])
{
    // To avoid warnings of unused parameters
    argc = argc;
    argv = argv;

    app_is_running = initialize_window();
    
    setup();

    while (app_is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    
    return EXIT_SUCCESS;
}

int initialize_window(void)
{
    LOG_SDL_REVISION_DEBUG();
    LOG_SDL_DEBUG("Initializing subsystems...");
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != EXIT_SUCCESS)
    {
        LOG_SDL_ERR("There was an error initializing the SDL library.");
        return false;
    }   
    
    LOG_SDL_DEBUG("Initializing window.");
    
    window = SDL_CreateWindow(
        WINDOW_TITLE,
        POSITION_X,
        POSITION_Y,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        LOG_SDL_ERR("Error creating SDL window.");
        return false;
    }

    LOG_SDL_DEBUG("Initializing renderer.");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == NULL)
    {
        LOG_SDL_ERR("Error creating renderer.");
        return false;
    }

    return true;
}

void setup(void)
{
    // Set the ball
    ball.h = 20;
    ball.w = 20;
    ball.x = 0;
    ball.y = 0;
}

void destroy_window(void)
{
    LOG_SDL_DEBUG("Destroying renderer...");
    SDL_DestroyRenderer(renderer);

    LOG_SDL_DEBUG("Destroying window...");
    SDL_DestroyWindow(window);

    LOG_SDL_DEBUG("Destroying subsystem...");
    SDL_Quit();
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
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
    
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    last_frame_time = SDL_GetTicks();
    
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(window, &w, &h);

    if (ball.x >= (w - ball.w) || ball.x < 0.0f)
    {
        ball_speed_x_axis *= -1;
    }

    ball.x += ball_speed_x_axis * delta_time;

    if (ball.y >= (h - ball.h) || ball.y < 0.0f)
    {
        ball_speed_y_axis *= -1;
    }

    ball.y += ball_speed_y_axis * delta_time;

    double factor = 50;

    background_colour_R = SQUARE(cos(2 * PI * frame_number / (FPS * factor))) * 256;
    background_colour_G = SQUARE(cos(2 * PI * frame_number / (FPS * factor)) + 0.5) * 256;
    background_colour_B = SQUARE(cos(2 * PI * frame_number / (FPS * factor)) + 0.5) * 256;

    // reset frame number
    frame_number = frame_number > (factor * FPS) ? 0 : frame_number;
    frame_number++;
}

void render(void)
{
    SDL_SetRenderDrawColor(
        renderer, 
        background_colour_R, 
        background_colour_G, 
        background_colour_B, 
        background_colour_A);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(
        renderer, 
        255, 
        255, 
        255, 
        255);
    SDL_RenderFillRect(renderer, &ball);

    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
}