#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "raylib.h"

//#define TUTORIAL_DEBUG
#define ADD_CIRCLES
//#define ADD_TRIANGLES
//#define DEFINE_ACC

#define RAND(x) ((x) * ((float)rand()/(float)RAND_MAX))
#define RAND_SIMMETRIC(x) ((x)/2.0f - (RAND(x))/2.0f)
#define SQUARE(x) ((x)*(x))
#define SIGN(x) (((x) > 0) - ((x) < 0))
#define ABS(x) ((x) > 0 ? (x) : (-1 * (x)))

#define MAX_SUBSTEPS 8
#define MAX_NUMBER_QUADRANTS 4 
#define MAX_OBJECTS 300
#define MAX_SPEED 60
#define MIN_SPEED_ALLOWED 0
#define SPEED_FACTOR 2
#define MAX_ACC 2
#define DENSITY 2
#define ELASTIC_RATIO 0.9
#define ELASTIC_RATIO_WALL 1
#define MASS_FACTOR 2000
#define GRAVITY (9.8 / MASS_FACTOR)
#define FRICTION_FACTOR 0.0000001

typedef enum ENTITY_TYPE 
{
	ENTITY_TRIANGLE,
	ENTITY_RECTANGLE,
	ENTITY_CIRCLE
} ENTITY_TYPE;

typedef struct Entity
{
	int entity_id;
	ENTITY_TYPE type;
	Vector2 position;
	Vector2 speed;
	Vector2 acceleration;
	int radius;
	float mass;
	Color color;
	int quadrants[MAX_NUMBER_QUADRANTS]; 
	bool alive; 
} Entity; 

typedef struct Entities { size_t capacity;
	size_t count;
	Entity *store;
} Entities;

Color generate_random_color()
{
	int r = GetRandomValue(0, 255);
	int g = GetRandomValue(0, 255);
	int b = GetRandomValue(0, 255);

	return (Color){r, g, b, 255};
}

Entity generate_random_circle(int id)
{
	//int radius = RAND(10) + 10;
	int radius = GetRandomValue(2, 5);
	return (Entity){
		.entity_id = id,
		.type = ENTITY_CIRCLE,
		.color = generate_random_color(),
		.speed = (Vector2) { .x = RAND_SIMMETRIC(MAX_SPEED), .y = RAND_SIMMETRIC(MAX_SPEED)},
		.acceleration = (Vector2) {.x = RAND_SIMMETRIC(MAX_ACC), .y = RAND_SIMMETRIC(MAX_ACC)},
		.radius = radius,
		.mass = radius, //PI * SQUARE(radius) * DENSITY,
		.alive = true
	};
}

Entity generate_random_triangle(int id)
{
	int radius = RAND(10) + 10;
	return (Entity){
		.entity_id = id,
		.type = ENTITY_TRIANGLE,
		.color = generate_random_color(),
		.speed = (Vector2) { .x = RAND_SIMMETRIC(MAX_SPEED), .y = RAND_SIMMETRIC(MAX_SPEED)},
		.radius = radius,
		.mass = 0.5 * SQUARE(radius) * DENSITY,
		.alive = true
	};
}

float calculate_distance(Vector2 p1, Vector2 p2)
{
	//printf("p1.x = %f, p1.y = %f, p2.x = %f, p2.y = %f\n", p1.x, p1.y, p2.x, p2.y);
	return sqrtf(SQUARE(p1.x - p2.x) + SQUARE(p1.y - p2.y));
}

void get_entity_quadrants(const Entity *object, int width_quadrant, int height_quadrant, int quadrants_per_row, size_t *number_of_quadrants, int *quadrants)
{
	float left_x = object->position.x - object->radius;
	float up_y = object->position.y - object->radius;

	int start_x_quadrant = (object->position.x - object->radius) / width_quadrant;
	int end_x_quadrant = (object->position.x + object->radius) / width_quadrant;
	int start_y_quadrant = (object->position.y - object->radius) / height_quadrant;
	int end_y_quadrant = (object->position.y + object->radius) / height_quadrant;

#ifdef TUTORIAL_DEBUG
	printf("Start_x_quadrant :: %d, End_x_quadrant :: %d, Start_y_quadrant :: %d, End_y_quadrant :: %d\n", start_x_quadrant, end_x_quadrant, start_y_quadrant, end_y_quadrant);
#endif
	*number_of_quadrants = 0;
	for (int i = start_x_quadrant; i <= end_x_quadrant; ++i)
	{
		for (int j = start_y_quadrant; j <= end_y_quadrant; ++j)
		{
			quadrants[(*number_of_quadrants)] = j * quadrants_per_row + i;
			(*number_of_quadrants)++;
		}
	}
}

bool check_collisions(const Entity *object, const Entities *other_objects, Entity *objects_colliding)
{
	size_t count = 0;
	for (size_t j = 0; j < other_objects->count; ++j)
	{
		Entity *other_object = (other_objects->store + j);

		if (object->entity_id == other_object->entity_id) continue;
		
		float d = calculate_distance(object->position, other_object->position);
		if (d <= object->radius + other_object->radius)
		{
			objects_colliding[count] = *other_object;
			++count;
		}	
	}
	return count;
}

void draw_entity(const Entity *object)
{
	assert(object != NULL);
	
	switch (object->type) {
		case ENTITY_CIRCLE:
			DrawCircle(object->position.x, object->position.y, object->radius, object->color); 
		break;
		case ENTITY_TRIANGLE:
			DrawPoly(object->position, 3, object->radius, 0, object->color);
		break;
		case ENTITY_RECTANGLE:
		default:
			DrawPoly(object->position, 4, object->radius, 0, object->color);
	};
}

void setup(Entities *objects, int width, int height)
{
#ifdef ADD_TRIANGLES
	size_t num_triangles = 10;

	for(size_t i = 0; i < num_triangles; ++i)
	{
		Entity tr = generate_random_triangle(objects->count);
		tr.position = (Vector2) {
			.x = RAND(width),
			.y = RAND(height)
		};
		*(objects->store + objects->count) = tr;
		++objects->count;
	}
#endif // ADD_TRIANGLES

#ifdef TUTORIAL_DEBUG
	printf("Adding %d objects\n", objects->count);
#endif

#ifdef ADD_CIRCLES
	size_t num_circles = 300;
	Entity coll_objects[num_circles];

	for(size_t i = 0; i < num_circles; ++i)
	{
		Entity cl;
		bool collision = true;
		
		do
		{
			cl = generate_random_circle(objects->count);
			cl.position = (Vector2) {
				.x = RAND(width - 2 * cl.radius) + cl.radius, 
				.y = RAND(height - 2 * cl.radius) + cl.radius
			};
			collision = check_collisions(&cl, objects, coll_objects);
		} while (collision);
		
		*(objects->store + objects->count) = cl;
		++objects->count;
	}

#ifdef TUTORIAL_DEBUG
	printf("Adding %d objects\n", objects->count);
#endif

	for (size_t i = 0; i < objects->count; ++i)
	{
		Entity object = *(objects->store + i);
#ifdef TUTORIAL_DEBUG
		printf("Object data id %d, position {%f, %f}\n", 
			object.entity_id, object.position.x, object.position.y);
#endif
	}
	
	return;

#endif // ADD_CIRCLES
	   
	Entity circle_1 = (Entity){
		.entity_id = 0,
		.position = (Vector2) { .x = 50, .y = 50},
		.speed = { 0, 0 },
		.alive = true,
		.color = WHITE,
		.radius = 20,
		.mass = 10,
		.type = ENTITY_CIRCLE
	};	
	
	//Entity circle_2 = (Entity){
		//.position = (Vector2) { .x = width, .y = height/2},
		//.speed = { -200, 0 },
		//.alive = true,
		//.color = RED,
		//.radius = 20,
		//.mass = 5,
		//.type = ENTITY_CIRCLE
	//};	
	Entity circle_2 = (Entity){
		.entity_id = 1,
		.position = (Vector2) { .x = 400, .y = 300},
		.speed = { 0, 0 },
		.alive = true,
		.color = RED,
		.radius = 200,
		.mass = 10,
		.type = ENTITY_CIRCLE
	};	

	*(objects->store) = circle_1;
	*(objects->store + 1) = circle_2;
	objects->count = 2;

	return;
}

void update(Entities *objects, int width, int height, int width_quadrant, int height_quadrant, float time)
{
	float speed_factor = SPEED_FACTOR;
	bool collision[objects->count];
	int quadrants_per_row = ceilf(width/(float)width_quadrant);

	// Update the object speed
	for (size_t i = 0; i < objects->count; ++i)
	{
		Entity *object = (objects->store + i);

		Vector2 object_next_position = {0, 0};
		object_next_position.x = object->position.x + object->speed.x * time * speed_factor;
		object_next_position.y = object->position.y + object->speed.y * time * speed_factor;
#if 0
		printf("Next Position .x = %f :: .y = %f \n", object_next_position.x, object_next_position.y);
#endif

		collision[i] = false;
		// Check for collisions with other objects
		for (size_t j = i; j < objects->count; ++j)
		{
			if (i == j) continue;

			Entity *other_object = (objects->store + j);
			Vector2 other_object_next_position = {0, 0};
			other_object_next_position.x = other_object->position.x + other_object->speed.x * time * speed_factor;
			other_object_next_position.y = other_object->position.y + other_object->speed.y * time * speed_factor;
			
			float d = calculate_distance(object_next_position, other_object_next_position);
			//printf("[Object id] : %d, Checking collisions. Distance between objects : %f \n", object->entity_id, d);

			if (d <= object->radius + other_object->radius)
			{
				collision[i] = true;
				//printf("Collision happened\n");
				//printf("Distance %f :: Radius Object %d :: Radius Object %d\n", d, object->radius, other_object->radius);
				//printf("[Object id] : %d, Before Collision Object Speed .x = %f, .y = %f\n", object->entity_id, object->speed.x, object->speed.y);
				
				// v1 = (m1 - m2)/(m1 + m2) * u1 + 2 * m2/(m1 + m2) * u2	
				// v2 = (m2 - m1)/(m1 + m2) * u2 + 2 * m1/(m1 + m2) * u1	
				float total_mass = (object->mass + other_object->mass);
				float mass_ratio_object = ((object->mass - other_object->mass)/total_mass);
				
				Vector2 speed_temp = (Vector2) {.x = object->speed.x, .y = object->speed.y};
				object->speed.x = 		ELASTIC_RATIO * ((2*other_object->mass/total_mass)   * other_object->speed.x    + mass_ratio_object * object->speed.x);
				other_object->speed.x = ELASTIC_RATIO * ((2*object->mass/total_mass)		 * object->speed.x 			- mass_ratio_object * other_object->speed.x); 
			
				object->speed.y = 		ELASTIC_RATIO * ((2*other_object->mass/total_mass)   * other_object->speed.y    + mass_ratio_object * object->speed.y);
				other_object->speed.y = ELASTIC_RATIO * ((2*object->mass/total_mass)		 * object->speed.y 			- mass_ratio_object * other_object->speed.y); 

				//printf("[Object id] : %d, After Collision Object Speed .x = %f, .y = %f\n", object->entity_id, object->speed.x, object->speed.y);
				object->position.x = object_next_position.x + (other_object_next_position.x - object_next_position.x) / d * (object->radius + other_object->radius);
				other_object->position.x = other_object_next_position.x - (other_object_next_position.x - object_next_position.x) / d * (object->radius + other_object->radius);
				object->position.y = object_next_position.y + (other_object_next_position.y - object_next_position.y) / d * (object->radius + other_object->radius);
				other_object->position.y = other_object_next_position.y - (other_object_next_position.y - object_next_position.y) / d * (object->radius + other_object->radius);
			}	
			else
			{
				object->position.x += object->speed.x * time * speed_factor;
				object->position.y += object->speed.y * time * speed_factor;
			}
		}
#ifdef DEFINE_ACC		
		object->speed.x += object->acceleration.x;
		object->speed.y += object->acceleration.y;
		if (ABS(object->speed.x) < MIN_SPEED_ALLOWED) object->speed.x = 0;
		if (ABS(object->speed.y) < MIN_SPEED_ALLOWED) object->speed.y = 0;
#endif // DEFINE_ACC
		
		if (object->position.x > (width - object->radius) || object->position.x < object->radius)
		{
			object->speed.x *= -1 * ELASTIC_RATIO_WALL;
			//object->acceleration.x *= -1;
		}
		if (object->position.y > (height - object->radius) || object->position.y < object->radius)
		{
			object->speed.y *= -1 * ELASTIC_RATIO_WALL;
			//object->acceleration.y *= -1;
		}
	}
	// Update position
	#if 0
	for (size_t i = 0; i < objects->count; ++i)
	{
		Entity *object = objects->store + i;

		// Check for collisions with the window
		if (object->position.x > (width - object->radius) || object->position.x < object->radius)
		{
			object->speed.x *= -1 * ELASTIC_RATIO_WALL;
			//object->acceleration.x *= -1;
		}
		if (object->position.y > (height - object->radius) || object->position.y < object->radius)
		{
			object->speed.y *= -1 * ELASTIC_RATIO_WALL;
			//object->acceleration.y *= -1;
		}

		if (0) //if (!collision[i])
		{
			object->position.x += object->speed.x * time * speed_factor;
			object->position.y += object->speed.y * time * speed_factor;
		}

#ifdef DEFINE_ACC
		object->speed.y += object->mass * GRAVITY - SIGN(object->speed.y) * SQUARE(object->speed.y) * FRICTION_FACTOR * SQUARE(object->radius);
		object->speed.x -= SIGN(object->speed.x) * SQUARE(object->speed.x) * FRICTION_FACTOR * SQUARE(object->radius);
#endif
		size_t num_quadrants = 0;
		int quadrants[64];
		get_entity_quadrants(object, 200, 200, quadrants_per_row, &num_quadrants, quadrants);

#ifdef TUTORIAL_DEBUG
		printf("[Object id] -> %d", object->entity_id);
		for (size_t q = 0; q < num_quadrants; ++q)
		{
			printf(" Quadrant : %d", quadrants[q]);
		}
		printf("\n");
#endif

	}
#endif
}

void process_input(Entities *objects, int quadrants_per_row)
{
	if (IsKeyDown(KEY_SPACE))
	{
		for (size_t i = 0; i < objects->count; ++i)
		{
			Entity *object = objects->store + i;

			size_t num_quadrants = 0;
			int quadrants[64];
			get_entity_quadrants(object, 200, 200, quadrants_per_row, &num_quadrants, quadrants);

			printf("[Object id] -> %d", object->entity_id);
			for (size_t q = 0; q < num_quadrants; ++q)
			{
				printf(" Quadrant : %d", quadrants[q]);
			}
			printf("\n");
		}
	}
}

int main(void)
{
	Entities objects = {
		.capacity = MAX_OBJECTS,
		.count = 0
	};
	objects.store = malloc(sizeof(Entity) * MAX_OBJECTS);

	const int width = 1600;
	const int height = 900;
	const int width_quadrant = 200;
	const int height_quadrant = 200;
		
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(width, height, "Tutorial 2");

	setup(&objects, width, height);

#ifdef TUTORIAL_DEBUG
	printf("Number of objects %d\n", objects.count);
#endif

	for (size_t i = 0; i < objects.count; ++i)
	{
		Entity object = *(objects.store + i);
#ifdef TUTORIAL_DEBUG
		printf("Object data id %d, position {%f, %f}\n", 
			object.entity_id, object.position.x, object.position.y);
#endif
	}

	while(!WindowShouldClose())
	{
		process_input(&objects, ceilf(width/(float)width_quadrant));

		BeginDrawing();

		{
			float frame_time = GetFrameTime();

			ClearBackground(BLACK);

			for(size_t i = 0; i < objects.count; ++i)
			{
				Entity *object = (objects.store + i);
				if (object->alive) draw_entity(object);
			}	

			for(int substep = 0; substep < MAX_SUBSTEPS; ++substep)
			{
				update(&objects, width, height, width_quadrant, height_quadrant, frame_time/MAX_SUBSTEPS);
			}
		}

		EndDrawing();
	}

	free(objects.store);
	CloseWindow();
}
