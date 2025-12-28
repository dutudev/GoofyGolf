#include <iostream>
#include "raylib.h"
#include "map.hpp"
#include "ball.hpp"

int main() {

	InitWindow(800, 600, "Goofy Golf");

	Texture groundTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/tilesetmap.png").c_str());
	Map currentMap;
	currentMap.LoadMap("testMap");
	Ball ball;
	ball.SetPosition(currentMap.GetBallStartPos());
	while (!WindowShouldClose()) {

		BeginDrawing();
		/*
		Rectangle rect = { 100, 0, 50, 50 };
		DrawTextureRec(groundTexture, rect, { 0, 0 }, WHITE);
		*/
		ClearBackground(BLACK);
		currentMap.DrawMap(groundTexture);
		ball.Draw();
		EndDrawing();

	}

	UnloadTexture(groundTexture);

	return 0;
}