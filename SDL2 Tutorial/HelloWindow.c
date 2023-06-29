#include "arcadia_sdl.h"
#include "SDL.h"
#include "SDL_version.h"
#include "stdbool.h"
#include "constants.h"
#include "math.h"

#define EQ_FLOAT(x, y, epsilon) (((x) - (y)) < epsilon ? true : false)
#define SQUARED(x) ((x) * (x))
#define WINDOW_TITLE "Hello Window!"
#define POSITION_X SDL_WINDOWPOS_CENTERED
#define POSITION_Y SDL_WINDOWPOS_CENTERED
#define WIDTH 600
#define HEIGHT 500
#define MAX_NUMBER_OF_POINTS (WIDTH * HEIGHT)
#define NUMBER_OF_POINTS_PER_PLOT (WIDTH)
#define PIXELS_PER_UNIT (WIDTH / 5)

int app_is_running = false;

SDL_Window *window;
SDL_Renderer *renderer;
int window_width = WIDTH;
int window_height = HEIGHT;

int last_frame_time = 0;
int frame_number = 0;

// Objects
// Define a matrix for storing all the number
SDL_FPoint points[MAX_NUMBER_OF_POINTS];

void setup(void);

void process_input(void);

void update(void);

void render(void);

void calculate_points_cos(
    SDL_FPoint *points, 
    size_t number_of_points, 
    int width, 
    int height,
    int pixels_per_unit);

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

    // Render loop
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
    // TODO
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

    calculate_points_cos(
        points, 
        sizeof(points) / sizeof(SDL_FPoint),
        window_width,
        window_height,
        PIXELS_PER_UNIT);

    double factor = 50;

    // reset frame number
    frame_number = frame_number > (factor * FPS) ? 0 : frame_number;
    frame_number++;
}

void render(void)
{
    SDL_SetRenderDrawColor(
        renderer, 
        0, 
        0, 
        0, 
        255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(
        renderer, 
        255, 
        255, 
        255, 
        255);

    // Render axis and plot
    arcadia_sdl_render_axis(renderer, 2, PIXELS_PER_UNIT, window_width / PIXELS_PER_UNIT);

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

    SDL_RenderPresent(renderer);
}

void calculate_points_cos(
    SDL_FPoint *points, 
    size_t number_of_points, 
    int width, 
    int height,
    int pixels_per_unit)
{
    float x_t = 0, y_t = 0;

    for (size_t i = 0; i < number_of_points; ++i)
    {
        float x_p = ((float) i / (float) number_of_points) - 1 / 2.f;
        float y_p = pixels_per_unit * cos(2.f * PI * width / pixels_per_unit * x_p);

        arcadia_sdl_translate_point_to_axis(x_p, y_p, width, height, &x_t, &y_t);

        points[i] = (SDL_FPoint) { .x = x_t, .y = y_t };
    }
}

