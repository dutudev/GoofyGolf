#include <iostream>
#include <vector>
#include <string>
#include "raylib.h"
#include "raymath.h"
#include "map.hpp"
#include "ball.hpp"

#define MAXBALLVELOCITY 150.0f

using std::vector;
using std::string;

int main() {

	InitWindow(800, 600, "Goofy Golf");
	InitAudioDevice();
	//game logic vars
	bool isChoosingDir = false;
	int strokes = 0;
	Vector2 startMousePos = { 0, 0 }, currentMousePos = { 0, 0 }, moveBall, moveBallSmooth;
	Texture arrowTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/arrow.png").c_str());
	Texture groundTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/tilesetmap.png").c_str());
	Font font = LoadFont(((std::string)GetWorkingDirectory() + "/assets/fonts/RobotoMonoBold.ttf").c_str());
	Sound ballHit = LoadSound(((std::string)GetWorkingDirectory() + "/assets/sounds/ballHit.wav").c_str());
	Sound ballDrop = LoadSound(((std::string)GetWorkingDirectory() + "/assets/sounds/ballDrop.wav").c_str());
	vector<Rectangle> mapWalls;

	//ball & map vars
	Map currentMap;
	Vector2 holePos;
	Ball ball;
	currentMap.LoadMap("testMap", mapWalls);
	ball.SetPosition(currentMap.GetBallStartPos());
	ball.SetBallDropSound(&ballDrop);
	holePos = currentMap.GetHolePos();
	while (!WindowShouldClose()) {

		//logic
		if (!ball.IsMoving()) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				isChoosingDir = true;
				startMousePos = GetMousePosition();
				moveBallSmooth = { 0.0f, 0.0f };
			}
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isChoosingDir) {
				isChoosingDir = false;
				ball.SetVelocity(moveBall * 6.0f);
				strokes++;
				PlaySound(ballHit);
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
		}
		
		ball.Logic(mapWalls, holePos);

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


		//draw ui TODO: add alpha when ball is close
		string holesString = "Holes : ";
		string strokesString = "Strokes : " + std::to_string(strokes);
		DrawRectangleRounded({ -55.0f, 543.0f, MeasureTextEx(font, strokesString.c_str(), 20, 0).x + 75, 90.0f}, 0.5f, 2, {0, 0, 0, 100});
		DrawTextPro(font, holesString.c_str(), {10.0f, MeasureTextEx(font, holesString.c_str(), 20, 0).y / 2.0f + 545.0f}, {0, 0}, 0, 20, 0, WHITE);
		DrawTextPro(font, strokesString.c_str(), {10.0f, MeasureTextEx(font, strokesString.c_str(), 20, 0).y / 2.0f + 565.0f}, {0, 0}, 0, 20, 0, WHITE);

		//debug Drawing
		/*
		for (Rectangle rect : mapWalls) {
			//DrawRectangle(rect.x, rect.y, rect.width, rect.height, { 255, 255, 255, 150 });
			DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);
			DrawCircleLines(ball.GetMiddlePosition().x, ball.GetMiddlePosition().y, 15, RED);
			DrawCircleLines(currentMap.GetHolePos().x, currentMap.GetHolePos().y, 10, RED);
		}*/
		EndDrawing();

	}
	UnloadFont(font);
	UnloadSound(ballDrop);
	UnloadSound(ballHit);
	UnloadTexture(arrowTexture);
	UnloadTexture(groundTexture);
	CloseAudioDevice();
	return 0;
}