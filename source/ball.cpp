#pragma once
#include "raylib.h"
#include "raymath.h"
#include "ball.hpp"


Ball::Ball() {
	ballTexture = LoadTexture(((std::string)GetWorkingDirectory() + "/assets/images/ball.png").c_str());
}

Ball::~Ball() {
	UnloadTexture(ballTexture);
}

Vector2 Ball::GetPosition() {
	return position;
}

Vector2 Ball::GetMiddlePosition() {
	return position + Vector2{ 50.0f, 50.0f } / 2.0f;
}

void Ball::SetPosition(Vector2 posToSet) {
	position = posToSet;
}

void Ball::SetVelocity(Vector2 velToSet) {
	velocity = velToSet;
}

void Ball::Logic() {

}

void Ball::Draw() {
	DrawTexture(ballTexture, position.x, position.y, WHITE);
}