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

void InitMaps(vector<string>* maps) {
	maps->push_back("map1");
	maps->push_back("map2");
}

void SetMap(Map& currentMap, vector<string>& allMaps, int& currentHole, Ball& ball, Sound& ballDrop, vector<Rectangle>& mapWalls, Vector2& holePos) {
	mapWalls.clear();
	ball.Reset();
	currentMap.LoadMap(allMaps[currentHole - 1], mapWalls);
	ball.SetPosition(currentMap.GetBallStartPos());
	ball.SetBallDropSound(&ballDrop);
	holePos = currentMap.GetHolePos();
}

void NextMap(Map& currentMap, vector<string>& allMaps, int& currentHole, Ball& ball, Sound& ballDrop, vector<Rectangle>& mapWalls, Vector2& holePos, int& strokesTotal, int& strokes) {
	currentHole++;
	strokesTotal += strokes;
	strokes = 0;
	if (currentHole == allMaps.size() + 1) {
		//send to next map
		std::cout << "DEBUG: Finished maps\n";
		return;
	}
	SetMap(currentMap, allMaps, currentHole, ball, ballDrop, mapWalls, holePos);
}

inline float easeOutExpo(float x){
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

inline float easeInExpo(float x) {
	return x == 0 ? 0 : pow(2, 10 * x - 10);
}

int main() {
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(800, 600, "Goofy Golf");
	InitAudioDevice();
	bool ShouldExit = false;
	bool tutorial = true, tutorialExit = false;
	float tutorialP = 1;
	bool transition = false;
	float progressTransition = 0, needTransition = 0, progressUnclampedTransition = 0;
	SetExitKey(KEY_NULL);
	//game logic vars
	bool isChoosingDir = false;
	int strokes = 0;
	int strokesTotal = 0;
	int currentHole = 0;
	Vector2 startMousePos = { 0, 0 }, currentMousePos = { 0, 0 }, moveBall, moveBallSmooth;
	Texture arrowTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/arrow.png").c_str());
	Texture groundTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/tilesetmap.png").c_str());
	Texture goofyLogoTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/goofygolflogo.png").c_str());
	Texture tutorial1Texture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/tutorial1.png").c_str());
	Font font = LoadFontEx(((std::string)GetWorkingDirectory() + "/assets/fonts/RobotoMonoBold.ttf").c_str(), 128, 0, 0);
	Sound ballHit = LoadSound(((std::string)GetWorkingDirectory() + "/assets/sounds/ballHit.wav").c_str());
	Sound ballDrop = LoadSound(((std::string)GetWorkingDirectory() + "/assets/sounds/ballDrop.wav").c_str());
	vector<Rectangle> mapWalls;
	vector<string> allMaps;
	Shader mainMenuShader = LoadShader(0, ((std::string)GetWorkingDirectory() + "/assets/shaders/mainMenuBG.glsl").c_str());
	Shader transitionShader = LoadShader(0, ((std::string)GetWorkingDirectory() + "/assets/shaders/transition.glsl").c_str());
	int shaderTimeLoc = GetShaderLocation(mainMenuShader, "time");
	int shaderTransitionLoc = GetShaderLocation(transitionShader, "progress");
	//main menu vars
	int currentButton = 0, currentMenu = 0;
	float currentProgress = 1;
	Rectangle currentRect, lastRect, nextRect = {0, 0, 0, 0};
	if (nextRect.x == 0) { // made this so v2 and rect1 delete when out of scope
		Vector2 v2 = MeasureTextEx(font, "Start", 64, 0) / 2.0f;
		Rectangle rect1 = { 640 - v2.x - 10, 200 - v2.y - 5, v2.x * 2 + 20, v2.y * 2 + 10 };
		lastRect = rect1;
		currentRect = rect1;
		nextRect = rect1;
	}
	InitMaps(&allMaps);

	//ball & map vars
	Map currentMap;
	Vector2 holePos;
	Ball ball;
	//SetMap(currentMap, allMaps, currentHole, ball, ballDrop, mapWalls, holePos);
	while (!ShouldExit) {

		if (WindowShouldClose()) {
			ShouldExit = true;
		}

		if (transition) {
			if (progressUnclampedTransition >= 1.5f && needTransition == 1) {
				needTransition = 0;
				NextMap(currentMap, allMaps, currentHole, ball, ballDrop, mapWalls, holePos, strokesTotal, strokes);
			}
			if (needTransition == 1) {
				progressUnclampedTransition += GetFrameTime() / 0.5f;
			}
			else {
				progressUnclampedTransition -= GetFrameTime() / 0.5f;
				if (progressUnclampedTransition <= 0) {
					progressTransition = 0;
					transition = false;
				}
			}
			
			progressTransition = Clamp(progressUnclampedTransition, 0.0f, 1.0f);
		}

		if (currentHole == 0) {
			Vector2 v2 = MeasureTextEx(font, "Start", 64, 0) / 2.0f;
			Vector2 v4 = MeasureTextEx(font, "About", 64, 0) / 2.0f;
			Vector2 v6 = MeasureTextEx(font, "Exit", 64, 0) / 2.0f;
			Rectangle rect1 = { 640 - v2.x - 10, 200 - v2.y - 5, v2.x * 2 + 20, v2.y * 2 + 10 };
			Rectangle rect2 = { 640 - v4.x - 10, 300 - v4.y - 5, v4.x * 2 + 20, v4.y * 2 + 10 };
			Rectangle rect3 = { 640 - v6.x - 10, 400 - v2.y - 5, v6.x * 2 + 20, v6.y * 2 + 10 };
			switch (currentMenu) {
			case 0:
				
				if (IsKeyPressed(KEY_W)) {
					currentButton = (currentButton - 1 < 0) ? 2 : currentButton - 1;
					currentProgress = 0;
					lastRect = currentRect;
					switch (currentButton) {
					case 0:
						nextRect = rect1;
						break;
					case 1:
						nextRect = rect2;
						break;
					case 2:
						nextRect = rect3;
						break;
					}
				}
				if (IsKeyPressed(KEY_S)) {
					currentButton = (currentButton + 1 > 2) ? 0 : currentButton + 1;
					currentProgress = 0;
					lastRect = currentRect;
					switch (currentButton) {
					case 0:
						nextRect = rect1;
						break;
					case 1:
						nextRect = rect2;
						break;
					case 2:
						nextRect = rect3;
						break;
					}
				}
				if (IsKeyPressed(KEY_SPACE) && !transition) {
					if (tutorial) {
						tutorialExit = true;
					}
					else {
						switch (currentButton) {
						case 0:
							needTransition = 1;
							transition = true;
							break;
						case 1:
							currentMenu = 1;
							break;
						case 2:
							ShouldExit = true;
							break;
						}
					}
				}

				currentProgress += GetFrameTime() / 1.5f;
				currentProgress = Clamp(currentProgress, 0, 1);
				Vector2 vect1 = Vector2Lerp({ lastRect.x, lastRect.y }, { nextRect.x, nextRect.y }, easeOutExpo(currentProgress));
				Vector2 vect2 = Vector2Lerp({ lastRect.width, lastRect.height }, { nextRect.width, nextRect.height }, easeOutExpo(currentProgress));
				currentRect = { vect1.x, vect1.y, vect2.x, vect2.y };
				break;
			case 1:
				if (IsKeyPressed(KEY_SPACE)) {
					currentMenu = 0;
				}
			}
			
		}
		else {
			//logic game
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
			else if (ball.IsFinish()) {
				transition = true;
				needTransition = 1;
			}
			ball.Logic(mapWalls, holePos);
		}

		

		//drawing
		BeginDrawing();
		ClearBackground(BLACK);
		if (currentHole == 0) {
			float shaderTime = GetTime() * 0.12f; // slow down the shader
			SetShaderValue(mainMenuShader, shaderTimeLoc, &shaderTime, SHADER_UNIFORM_FLOAT);

			BeginShaderMode(mainMenuShader);
			DrawTexturePro(groundTexture, { 0, 0, 50, 50 }, { 0, 0, 800, 600 }, { 0, 0 }, 0, WHITE);
			EndShaderMode();
			Vector2 v1 = { 640, 200 };
			Vector2 v2 = MeasureTextEx(font, "Start", 64, 0) / 2.0f;
			Vector2 v3 = { 640, 300 };
			Vector2 v4 = MeasureTextEx(font, "About", 64, 0) / 2.0f;
			Vector2 v5 = { 640, 400 };
			Vector2 v6 = MeasureTextEx(font, "Exit", 64, 0) / 2.0f;
			Vector2 v7 = { 400, 585 };
			Vector2 v8 = MeasureTextEx(font, "Game made by dutudev :)", 20, 0) / 2.0f;
			Vector2 v9 = { 400, 100 };
			Vector2 v10 = MeasureTextEx(font, "About", 128, 0) / 2.0f;
			Vector2 v11 = { 400, 300 };
			Vector2 v12 = MeasureTextEx(font, "A little game about golf made in cpp using raylib\nPart of dutu's Nerve Collection\nTo learn more, watch my yt\nMade with <3 by dutudev", 32, 0) / 2.0f;
			switch (currentMenu) {
			case 0:
				

				DrawTextureEx(goofyLogoTexture, { 15, GetScreenHeight() / 2.0f - goofyLogoTexture.height - 25.0f + ((float)(sin(GetTime() * 3.5f)*sin(GetTime() * 3.5f))) / 2.0f * 25.0f }, 0, 2, WHITE);
				
				DrawTextPro(font, "Start", v1 - v2, { 0,0 }, 0, 64, 0, WHITE);
				
				DrawTextPro(font, "About", v3 - v4, { 0,0 }, 0, 64, 0, WHITE);
				
				DrawTextPro(font, "Exit", v5 - v6, { 0,0 }, 0, 64, 0, WHITE);
				
				DrawTextPro(font, "Game made by dutudev :)", v7 - v8, { 0,0 }, 0, 20, 0, WHITE);
				DrawRectangleRoundedLinesEx(currentRect, 0.2f, 2, (sin(GetTime() * 6.0f) + 1) / 2.0f * 4 + 2, WHITE);
				if (tutorial) {
					
					if (tutorialExit) {
						tutorialP -= GetFrameTime();
					}
					DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 35,35,35,(unsigned char)(255 * tutorialP) });
					DrawTextPro(font, "How to play", Vector2{ 400, 50 } - MeasureTextEx(font, "How to play", 64, 0) / 2.0f, { 0,0 }, 0, 64, 0, { 255,255,255,(unsigned char)(255 * tutorialP) });
					DrawTextPro(font, "Click and drag to aim, then release to hit the ball", Vector2{ 400, 150 } - MeasureTextEx(font, "Click and drag to aim, then release to hit the ball", 32, 0) / 2.0f, { 0,0 }, 0, 32, 0, { 255,255,255,(unsigned char)(255 * tutorialP) });
					DrawTexture(tutorial1Texture, 400 - tutorial1Texture.width / 2.0f, 170, { 255,255,255,(unsigned char)(255 * tutorialP) });
					DrawTextPro(font, "To navigate menus, use W (up), S (down), SPACE (confirm)", Vector2{ 400, 300 } - MeasureTextEx(font, "To navigate menus, use W (up), S (down), SPACE (confirm)", 30, 0) / 2.0f, { 0,0 }, 0, 30, 0, { 255,255,255,(unsigned char)(255 * tutorialP) });
					DrawTextPro(font, "Press Space to continue", Vector2{ 400, 390 } - MeasureTextEx(font, "Press Space to continue", 32, 0) / 2.0f, { 0,0 }, 0, 32, 0, { 255,255,255,(unsigned char)(255 * tutorialP) });
					if (tutorialP <= 0) {
						tutorial = false;
					}
				}
				break;
			case 1:
				
				DrawTextPro(font, "About", v9 - v10, { 0,0 }, 0, 128, 0, WHITE);
				DrawTextPro(font, "A little game about golf made in cpp using raylib\nPart of dutu's Nerve Collection\nTo learn more, watch my yt\nMade with <3 by dutudev", v11 - v12, { 0,0 }, 0, 32, 0, WHITE);
				DrawTextPro(font, "Back To Main Menu", Vector2{ 400, 525 } - MeasureTextEx(font, "Back To Main Menu", 64, 0) / 2.0f, {0,0}, 0, 64, 0, WHITE);
				DrawRectangleRoundedLinesEx({ (Vector2{ 400, 525 } - MeasureTextEx(font, "Back To Main Menu", 64, 0)/2.0f).x-10, (Vector2{ 400, 525 } - MeasureTextEx(font, "Back To Main Menu", 64, 0)/2.0f).y-5, MeasureTextEx(font, "Back To Main Menu", 64, 0).x + 20, MeasureTextEx(font, "Back To Main Menu", 64, 0).y + 10 }, 0.2f, 2, (sin(GetTime() * 6.0f) + 1) / 2.0f * 4 + 2, WHITE); // got tired of declaring vectors but this is awful
				break;
			}
			
		}
		else {
			currentMap.DrawMap(groundTexture);
			if (isChoosingDir) {
				DrawLineEx(ball.GetMiddlePosition(), ball.GetMiddlePosition() + moveBallSmooth, 4, BLACK);
				DrawTexturePro(arrowTexture, { 0.0f, 0.0f, (float)arrowTexture.height, (float)arrowTexture.width }, { (ball.GetMiddlePosition() + moveBallSmooth).x, (ball.GetMiddlePosition() + moveBallSmooth).y, (float)arrowTexture.height, (float)arrowTexture.width }, { (float)arrowTexture.height / 2.0f, (float)arrowTexture.width / 2.0f }, atan2((moveBallSmooth).y, (moveBallSmooth).x) * 180.0f / PI + 90.0f, WHITE);
			}
			//DrawCircle(GetMouseX(), GetMouseY(), 4, RED);
			ball.Draw();


			//draw ui TODO: add alpha when ball is close
			string holesString = "Hole : " + std::to_string(currentHole);
			string strokesString = "Strokes : " + std::to_string(strokes);
			DrawRectangleRounded({ -55.0f, 543.0f, MeasureTextEx(font, strokesString.c_str(), 20, 0).x + 75, 90.0f }, 0.5f, 2, { 0, 0, 0, 100 });
			DrawTextPro(font, holesString.c_str(), { 10.0f, MeasureTextEx(font, holesString.c_str(), 20, 0).y / 2.0f + 545.0f }, { 0, 0 }, 0, 20, 0, WHITE);
			DrawTextPro(font, strokesString.c_str(), { 10.0f, MeasureTextEx(font, strokesString.c_str(), 20, 0).y / 2.0f + 565.0f }, { 0, 0 }, 0, 20, 0, WHITE);

			//debug Drawing
			/*
			for (Rectangle rect : mapWalls) {
				//DrawRectangle(rect.x, rect.y, rect.width, rect.height, { 255, 255, 255, 150 });
				DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);
				DrawCircleLines(ball.GetMiddlePosition().x, ball.GetMiddlePosition().y, 15, RED);
				DrawCircleLines(currentMap.GetHolePos().x, currentMap.GetHolePos().y, 10, RED);
			}*/
		}

		//transitionDrawing on top all of others cause it is easier
	
		if (transition) {
			float toSend;
			if (needTransition == 1) {
				toSend = easeOutExpo(progressTransition);
				//std::cout << toSend << '\n';
				SetShaderValue(transitionShader, shaderTransitionLoc, &toSend, SHADER_UNIFORM_FLOAT);
			}
			else {
				toSend = easeOutExpo(progressTransition);
				//std::cout << toSend << '\n';
				SetShaderValue(transitionShader, shaderTransitionLoc, &toSend, SHADER_UNIFORM_FLOAT);
			}
			BeginShaderMode(transitionShader);
			DrawTexturePro(arrowTexture, { 0, 0, (float)arrowTexture.width, (float)arrowTexture.height }, { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0, 0 }, 0, BLACK);
			EndShaderMode();
		}
		EndDrawing();
		

	}
	UnloadShader(mainMenuShader);
	UnloadShader(transitionShader);
	UnloadFont(font);
	UnloadSound(ballDrop);
	UnloadSound(ballHit);
	UnloadTexture(arrowTexture);
	UnloadTexture(groundTexture);
	UnloadTexture(goofyLogoTexture);
	UnloadTexture(tutorial1Texture);
	CloseAudioDevice();
	return 0;
}