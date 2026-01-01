#include <iostream>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "map.hpp"
#include "ball.hpp"

#define MAXBALLVELOCITY 150.0f

using std::vector;

int main() {

	InitWindow(800, 600, "Goofy Golf");

	//game logic vars
	bool isChoosingDir = false;
	Vector2 startMousePos = { 0, 0 }, currentMousePos = { 0, 0 }, moveBall, moveBallSmooth;
	Texture arrowTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/arrow.png").c_str());
	Texture groundTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/tilesetmap.png").c_str());
	vector<Rectangle> mapWalls;

	//ball & map vars
	Map currentMap;
	Ball ball;
	currentMap.LoadMap("testMap", mapWalls);
	ball.SetPosition(currentMap.GetBallStartPos());
	while (!WindowShouldClose()) {

		//logic
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			isChoosingDir = true;
			startMousePos = GetMousePosition();
			moveBallSmooth = { 0.0f, 0.0f};
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			isChoosingDir = false;
			ball.SetVelocity(moveBall * 6.0f);
		}
		if (isChoosingDir) {
			currentMousePos = GetMousePosition();
			moveBall = currentMousePos - startMousePos;
			moveBall *= 1.4f;
			if (Vector2Length(moveBall) >= MAXBALLVELOCITY) {
				moveBall = Vector2Normalize(moveBall) * MAXBALLVELOCITY;
			}
			moveBall *= -1;
			moveBallSmooth = Vector2Lerp(moveBallSmooth, moveBall, 6.0f * GetFrameTime());
		}
		ball.Logic(mapWalls);

		//drawing
		BeginDrawing();
		ClearBackground(BLACK);
		currentMap.DrawMap(groundTexture);
		if (isChoosingDir) {
			DrawLineEx(ball.GetMiddlePosition(), ball.GetMiddlePosition() + moveBallSmooth, 4, BLACK);
			DrawTexturePro(arrowTexture, { 0.0f, 0.0f, (float)arrowTexture.height, (float)arrowTexture.width }, { (ball.GetMiddlePosition() + moveBallSmooth).x, (ball.GetMiddlePosition() + moveBallSmooth).y, (float)arrowTexture.height, (float)arrowTexture.width}, { (float)arrowTexture.height / 2.0f, (float)arrowTexture.width / 2.0f }, atan2((moveBallSmooth).y, (moveBallSmooth).x) * 180.0f/PI + 90.0f, WHITE);
		}
		//DrawCircle(GetMouseX(), GetMouseY(), 4, RED);
		ball.Draw();

		//debug Drawing
		/*
		for (Rectangle rect : mapWalls) {
			//DrawRectangle(rect.x, rect.y, rect.width, rect.height, { 255, 255, 255, 150 });
			DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);
			DrawCircleLines(ball.GetMiddlePosition().x, ball.GetMiddlePosition().y, 15, RED);
		}*/
		EndDrawing();

	}
	UnloadTexture(arrowTexture);
	UnloadTexture(groundTexture);

	return 0;
}