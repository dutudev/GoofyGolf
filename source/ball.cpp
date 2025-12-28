#pragma once
#include "raylib.h"
#include "ball.hpp"


Ball::Ball() {
	ballTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/ball.png").c_str());
}

Ball::~Ball() {
	UnloadTexture(ballTexture);
}

void Ball::SetPosition(Vector2 posToSet) {
	position = posToSet;
}

void Ball::Draw() {
	DrawTexture(ballTexture, position.x, position.y, WHITE);
}