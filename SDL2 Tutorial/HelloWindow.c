#include "stdio.h"
#include "SDL.h"
#include "SDL_version.h"
#include "stdbool.h"
#include "constants.h"
#include "math.h"
#include "arcadia_sdl.h"

#define WINDOW_TITLE "Hello Window!"
#define POSITION_X SDL_WINDOWPOS_CENTERED
#define POSITION_Y SDL_WINDOWPOS_CENTERED
#define WIDTH 600
#define HEIGHT 500
#define MAX_NUMBER_OF_POINTS (WIDTH * HEIGHT)
#define NUMBER_OF_POINTS_PER_PLOT (WIDTH * 10)
#define PIXELS_PER_UNIT (WIDTH / 5)

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
SDL_FPoint points[NUMBER_OF_POINTS_PER_PLOT];

void setup(void);

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
    
    setup();

    while (app_is_running)
    {
        process_input();
        update();
        render();
    }

    arcadia_sdl_destroy_window(&window, &renderer);
    
    return EXIT_SUCCESS;
}


void setup(void)
{
    // Set the ball
    ball.h = 20;
    ball.w = 20;
    ball.x = 0;
    ball.y = 0;

    int w, h;
    float x_out = 0.f, y_out = 0.f;
    SDL_GetWindowSize(window, &w, &h);

    float plot_factor = w / NUMBER_OF_POINTS_PER_PLOT;

    for (int i = 0; i < NUMBER_OF_POINTS_PER_PLOT; ++i)
    {
        points[i] = (SDL_FPoint) {
            .x = (i - NUMBER_OF_POINTS_PER_PLOT / 2.f) * w / NUMBER_OF_POINTS_PER_PLOT + w / 2.f,
            .y = h / 2.f - PIXELS_PER_UNIT * cos(2 * PI * w / PIXELS_PER_UNIT * (i - NUMBER_OF_POINTS_PER_PLOT / 2.f) / NUMBER_OF_POINTS_PER_PLOT)
        };
    }
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
    SDL_SetRenderDrawColor(
        renderer, 
        255, 
        255, 
        255, 
        255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(
        renderer, 
        255, 
        255, 
        255, 
        255);
    SDL_RenderFillRect(renderer, &ball);

    SDL_SetRenderDrawColor(
        renderer, 
        0, 
        0, 
        0, 
        255);

    arcadia_sdl_render_triangle(
        renderer,
        WIDTH/2.0f , HEIGHT/2.0f - 50.f, 
        WIDTH/2.0f + 100.0f - 50.f, HEIGHT/2.0f + 100.0f - 50.f,
        WIDTH/2.0f - 50.f, HEIGHT/2.0f + 100.0f - 50.f
    );

    arcadia_sdl_render_axis(renderer, 2);

    SDL_SetRenderDrawColor(
        renderer, 
        255, 
        0, 
        0, 
        255);

    SDL_RenderDrawLinesF(
        renderer,
        points,
        sizeof(points) / sizeof(SDL_FPoint)
    );

    //SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
}
