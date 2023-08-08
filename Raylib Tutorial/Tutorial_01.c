//
// This program simulates the GUI of a sound equalizer.
// The different rectangles heights are randomly generated.
//

#include <stdlib.h>
#include "raylib.h"

int main(void)
{
	const int screen_width = 1200;
	const int screen_height = 900;

	InitWindow(screen_width, screen_height, "Hello Raylib!");

	SetExitKey(KEY_ESCAPE);

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		{
			ClearBackground(BLACK);

			const unsigned int total_rect = 100;
			const int rect_width = screen_width / total_rect;
			 
			for (size_t num_rect = 0; num_rect <= total_rect; ++num_rect)
			{
				Rectangle rect = (Rectangle) {
					.x = num_rect * rect_width,
					.y = screen_height/2,
					.width = screen_width / total_rect,
					.height = GetRandomValue(10, 100)
				};
				
				DrawRectanglePro(rect, (Vector2){0, 0}, 180, RED);
			};

			WaitTime(0.2);
		}

		EndDrawing();
	}
	
	CloseWindow();
	
}