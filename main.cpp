#include "raylib.h"

int main() {

	InitWindow(800, 600, "Goofy Golf");

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();

	}

	return 0;
}