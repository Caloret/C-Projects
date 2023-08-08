#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "raylib.h"

#define TUTORIAL_DEBUG
#define ADD_CIRCLES
//#define ADD_TRIANGLES

#define RAND(x) ((x) * ((float)rand()/(float)RAND_MAX))
#define RAND_SIMMETRIC(x) ((x)/2.0f - (RAND(x))/2.0f)
#define SQUARE(x) ((x)*(x))
#define SIGN(x) (((x) > 0) - ((x) < 0))
#define ABS(x) ((x) > 0 ? (x) : (-1 * (x)))

#define MAX_OBJECTS 300
#define MAX_SPEED 60
#define SPEED_FACTOR 2
#define MAX_ACC 2
#define DENSITY 2
#define ELASTIC_RATIO 0.5
#define MASS_FACTOR 800
#define GRAVITY 0 // (9.8 / MASS_FACTOR)

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
	bool alive;
} Entity;

typedef struct Entities
{
	size_t capacity;
	size_t count;
	Entity *store;
} Entities;

Entity generate_random_circle(int id)
{
	int radius = RAND(10) + 10;
	return (Entity){
		.entity_id = id,
		.type = ENTITY_CIRCLE,
		.color = WHITE,
		.speed = (Vector2) { .x = RAND_SIMMETRIC(MAX_SPEED), .y = RAND_SIMMETRIC(MAX_SPEED)},
		.acceleration = (Vector2) {.x = RAND_SIMMETRIC(MAX_ACC), .y = RAND_SIMMETRIC(MAX_ACC)},
		.radius = radius,
		.mass = PI * SQUARE(radius) * DENSITY,
		.alive = true
	};
}

Entity generate_random_triangle(int id)
{
	int radius = RAND(10) + 10;
	return (Entity){
		.entity_id = id,
		.type = ENTITY_TRIANGLE,
		.color = RED,
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
	size_t num_triangles = 20;

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
	size_t num_circles = 15;
	Entity coll_objects[num_circles];

	for(size_t i = 0; i < num_circles; ++i)
	{
		Entity cl;
		bool collision = true;
		
		do
		{
			cl = generate_random_circle(objects->count);
			cl.position = (Vector2) {
				.x = RAND(width),
				.y = RAND(height)
			};
			collision = check_collisions(&cl, objects, coll_objects);
		} while (collision);
		
		*(objects->store + objects->count) = cl;
		++objects->count;
	}
#endif // ADD_CIRCLES

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

	Entity circle_1 = (Entity){
		.position = (Vector2) { .x = 0, .y = height/2},
		.speed = { 200, 0 },
		.alive = true,
		.color = WHITE,
		.radius = 20,
		.mass = 10,
		.type = ENTITY_CIRCLE
	};	
	
	Entity circle_2 = (Entity){
		.position = (Vector2) { .x = width, .y = height/2},
		.speed = { -200, 0 },
		.alive = true,
		.color = RED,
		.radius = 20,
		.mass = 20,
		.type = ENTITY_CIRCLE
	};	

	*(objects->store) = circle_1;
	*(objects->store + 1) = circle_2;
	objects->count = 2;

	return;
}

void update(Entities *objects, int width, int height)
{
	float frame_time = GetFrameTime();
	float speed_factor = SPEED_FACTOR;
	bool collision[objects->count];

	// Update the object speed
	for (size_t i = 0; i < objects->count; ++i)
	{
		Entity *object = (objects->store + i);

		Vector2 object_next_position = {0, 0};
		object_next_position.x = object->position.x + object->speed.x * frame_time * speed_factor;
		object_next_position.y = object->position.y + object->speed.y * frame_time * speed_factor;
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
			other_object_next_position.x = other_object->position.x + other_object->speed.x * frame_time * speed_factor;
			other_object_next_position.y = other_object->position.y + other_object->speed.y * frame_time * speed_factor;
			
			float d = calculate_distance(object_next_position, other_object_next_position);
			//printf("[Object id] : %d, Checking collisions. Distance between objects : %f \n", object->entity_id, d);

			if (d <= object->radius + other_object->radius)
			{
				collision[i] = true;
				//printf("Collision happened\n");
				//printf("Distance %f :: Radius Object %d :: Radius Object %d\n", d, object->radius, other_object->radius);
				//printf("[Object id] : %d, Before Collision Object Speed .x = %f, .y = %f\n", object->entity_id, object->speed.x, object->speed.y);
				
				// v1 = (m1 - m2)/(m1 + m2) * u1	
				// v2 = 2m1/(m1 + m2) * u1	
				float mass_ratio_object = ABS((object->mass - other_object->mass)/(object->mass + other_object->mass)) * ELASTIC_RATIO;
				float mass_ratio_other_object = ABS(2*object->mass/(object->mass + other_object->mass)) * ELASTIC_RATIO;
				
				Vector2 speed_temp = (Vector2) {.x = object->speed.x, .y = object->speed.y};
				if (SIGN(object->speed.x) != SIGN(other_object->speed.x))
				{
					object->speed.x = -1 * mass_ratio_object * speed_temp.x;					
					other_object->speed.x = mass_ratio_other_object * speed_temp.x;					
				}
				if (SIGN(object->speed.y) != SIGN(other_object->speed.y))
				{
					object->speed.y = -1 * mass_ratio_object * speed_temp.y;					
					other_object->speed.y = mass_ratio_other_object * speed_temp.y;					
				}

				//printf("[Object id] : %d, After Collision Object Speed .x = %f, .y = %f\n", object->entity_id, object->speed.x, object->speed.y);
			}	
		}
#ifdef DEFINE_ACC		
		object->speed.x += object->acceleration.x;
		object->speed.y += object->acceleration.y;
#endif // DEFINE_ACC
	}

	// Update position
	for (size_t i = 0; i < objects->count; ++i)
	{
		Entity *object = objects->store + i;

		// Check for collisions with the window
		if (object->position.x > width || object->position.x < 0)
		{
			object->speed.x *= -1;
			object->acceleration.x *= -1;
		}
		if (object->position.y > height || object->position.y < 0)
		{
			object->speed.y *= -1;
			object->acceleration.y *= -1;
		}

		if (!collision[i])
		{
			object->position.x += object->speed.x * frame_time * speed_factor;
			object->position.y += object->speed.y * frame_time * speed_factor;
		}

#ifdef DEFINE_ACC
		object->speed.y += object->mass * GRAVITY;
#endif
	}
}

int main(void)
{
	Entities objects = {
		.capacity = MAX_OBJECTS,
		.count = 0
	};
	objects.store = malloc(sizeof(Entity) * MAX_OBJECTS);

	const int width = 800;
	const int height = 600;
	
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
		BeginDrawing();

		{
			ClearBackground(BLACK);

			for(size_t i = 0; i < objects.count; ++i)
			{
				Entity *object = (objects.store + i);
				if (object->alive) draw_entity(object);
			}	

			update(&objects, width, height);
		}

		EndDrawing();
	}

	free(objects.store);
	CloseWindow();
}
