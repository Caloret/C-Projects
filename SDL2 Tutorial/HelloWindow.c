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
SDL_FPoint cos_points[MAX_NUMBER_OF_POINTS];
SDL_FPoint atan_points[MAX_NUMBER_OF_POINTS];
SDL_FPoint circle_points[MAX_NUMBER_OF_POINTS];

void setup(void);

void process_input(void);

void update(void);

void render(void);

void update_points(void);

void calculate_points_cos(
    SDL_FPoint *points, 
    size_t number_of_points, 
    int width, 
    int height,
    int pixels_per_unit);

void calculate_points_atan(
    SDL_FPoint *points, 
    size_t number_of_points, 
    int width, 
    int height,
    int pixels_per_unit);

void calculate_points_circle(
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
    update_points();
}

void update_points(void)
{
    calculate_points_cos(
        cos_points, 
        sizeof(cos_points) / sizeof(SDL_FPoint),
        window_width,
        window_height,
        PIXELS_PER_UNIT);

    calculate_points_atan(
        atan_points, 
        sizeof(atan_points) / sizeof(SDL_FPoint),
        window_width,
        window_height,
        PIXELS_PER_UNIT);

    calculate_points_circle(
        circle_points, 
        sizeof(circle_points) / sizeof(SDL_FPoint),
        window_width,
        window_height,
        PIXELS_PER_UNIT);
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
                update_points();
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

    SDL_RenderDrawPointsF(
        renderer,
        cos_points,
        sizeof(cos_points) / sizeof(SDL_FPoint)
    );

    SDL_SetRenderDrawColor(
        renderer, 
        0, 
        255, 
        0, 
        255);

    SDL_RenderDrawPointsF(
        renderer,
        atan_points,
        sizeof(atan_points) / sizeof(SDL_FPoint)
    );

    SDL_SetRenderDrawColor(
        renderer, 
        0, 
        0, 
        255, 
        255);

    SDL_RenderDrawPointsF(
        renderer,
        circle_points,
        sizeof(circle_points) / sizeof(SDL_FPoint)
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
        float x_p = width * (((float) i / (float) number_of_points) - 1 / 2.f);
        float y_p = pixels_per_unit * cos(2.f * PI * x_p / pixels_per_unit);

        arcadia_sdl_translate_point_to_axis(x_p, y_p, width, height, &x_t, &y_t);

        points[i] = (SDL_FPoint) { .x = x_t, .y = y_t };
    }
}

void calculate_points_atan(
    SDL_FPoint *points, 
    size_t number_of_points, 
    int width, 
    int height,
    int pixels_per_unit)
{
    float x_t = 0, y_t = 0;

    for (size_t i = 0; i < number_of_points; ++i)
    {
        float x_p = width * (((float) i / (float) number_of_points) - 1 / 2.f);
        float y_p = pixels_per_unit * atanf(2.f * PI * x_p / pixels_per_unit);

        arcadia_sdl_translate_point_to_axis(x_p, y_p, width, height, &x_t, &y_t);

        points[i] = (SDL_FPoint) { .x = x_t, .y = y_t };
    }
}

void calculate_points_circle(
    SDL_FPoint *points, 
    size_t number_of_points, 
    int width, 
    int height,
    int pixels_per_unit)
{
    float x_t = 0, y_t = 0;
    float radius = 2;

    size_t total_count = 0; 
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            total_count = i * height + j;

            if (total_count >= number_of_points) return;

            arcadia_sdl_translate_point_to_axis(i, j, width, height, &x_t, &y_t);

            bool is_circle_point = (SQUARED(i) + SQUARED(j)) - SQUARED(radius * pixels_per_unit) <= 0;
            
            //printf("i = %d :: j = %d :: x = %f :: y = %f \n", i, j, x_t, y_t);

            if (is_circle_point)
            {
                points[total_count] = (SDL_FPoint) { .x = i + width / 2, .y = j + height / 2 };
            } 
        }    
    }

    //exit(0);
}
