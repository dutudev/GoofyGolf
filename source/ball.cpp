#pragma once
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "ball.hpp"

using std::vector;

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

void Ball::Logic(vector<Rectangle> mapWalls) {
	if (Vector2Length(velocity) - decedeceleration * GetFrameTime() <= 0) {
		velocity = Vector2Zero();
	}
	else {
		velocity = Vector2Normalize(velocity) * (Vector2Length(velocity) - decedeceleration * GetFrameTime());
	}
	position += velocity * GetFrameTime();
	

	// collision for window screen
	if (position.x + 10 <= 0 && velocity.x <= 0) {
		velocity.x *= -1;
	}

	if (position.x + 40 >= GetScreenWidth() && velocity.x >= 0) {
		velocity.x *= -1;
	}

	if (position.y + 10 <= 0 && velocity.y <= 0) {
		velocity.y *= -1;
	}

	if (position.y + 40 >= GetScreenHeight() && velocity.y >= 0) {
		velocity.y *= -1;
	}

	//collision for map walls
	for (Rectangle wall : mapWalls) {
		if (CheckCollisionCircleRec(GetMiddlePosition(), 15, wall)) {

			//velocity = Vector2Zero();
			Vector2 vectMove = position - Vector2{ wall.x, wall.y };
			bool flip = abs(vectMove.x) > abs(vectMove.y) ? false : true;
			while (CheckCollisionCircleRec(GetMiddlePosition(), 15, wall)) {
				position += Vector2Normalize(vectMove);
			}
			if (flip) {
				velocity.y *= -1;
			}
			else {
				velocity.x *= -1;
			}

			/*if (abs(wall.x + 25.0f - GetMiddlePosition().x) > abs(wall.y + 25.0f - GetMiddlePosition().y) && abs(velocity.y) / velocity.y != abs(wall.y + 25.0f - GetMiddlePosition().y) / wall.y + 25.0f - GetMiddlePosition().y) {
				velocity.y *= -1;
			}
			else if(abs(wall.x + 25.0f - GetMiddlePosition().x) < abs(wall.y + 25.0f - GetMiddlePosition().y) && abs(velocity.x) / velocity.x != abs(wall.x + 25.0f - GetMiddlePosition().x) / wall.x + 25.0f - GetMiddlePosition().x) {
				velocity.x *= -1;
			}*/
		}
	}
}

void Ball::Draw() {
	DrawTexture(ballTexture, position.x, position.y, WHITE);
}