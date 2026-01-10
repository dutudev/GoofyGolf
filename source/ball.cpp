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
	ballDrop = 0;
	UnloadTexture(ballTexture);
}

Vector2 Ball::GetPosition() {
	return position;
}

Vector2 Ball::GetMiddlePosition() {
	return position + Vector2{ 50.0f, 50.0f } / 2.0f;
}

bool Ball::GetHitHole() {
	return hitHole;
}

bool Ball::IsMoving() {
	if (Vector2Length(velocity) > 0.2) {
		return true;
	}
	return false;
}

bool Ball::IsFinish() {
	if (hitProgress <= 0) {
		return true;
	}
	return false;
}

void Ball::Reset() {
	hitHole = false;
	hitProgress = 1.5f;
	position = { 0, 0 };
	velocity = { 0, 0 };
	hitPosition = { 0, 0 };
}

void Ball::SetBallDropSound(Sound* sound) {
	ballDrop = sound;
}

void Ball::SetPosition(Vector2 posToSet) {
	position = posToSet;
}

void Ball::SetVelocity(Vector2 velToSet) {
	velocity = velToSet;
}

void Ball::Logic(vector<Rectangle>& mapWalls, Vector2& holePos) {
	if (hitHole) {
		hitProgress -= GetFrameTime() / 0.35f;
		hitProgress = Clamp(hitProgress, 0.0f, 1.0f);
		position = Vector2Lerp(holePos, hitPosition, hitProgress / 1.5f);
		return;
	}

	for (int i = 0; i < precision && !hitHole; i++) {
		if (Vector2Length(velocity) - decedeceleration * GetFrameTime() <= 0) {
			velocity = Vector2Zero();
		}
		else {
			velocity = Vector2Normalize(velocity) * (Vector2Length(velocity) - decedeceleration * GetFrameTime() / precision);
		}
		position += velocity * GetFrameTime() / precision;

		//check to see if it hit hole
		if (CheckCollisionCircles(GetMiddlePosition(), 15, holePos, 10)) {
			hitHole = true;
			hitPosition = position;
			PlaySound(*ballDrop);
			return;
		}


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

				//check if it collided on x or y
				if (CheckCollisionCircleRec({ GetMiddlePosition().x, lastPosition.y }, 15, wall)) {
					velocity.x *= -1;
					position.x = lastPosition.x - 25.0f;
				}

				if (CheckCollisionCircleRec({ lastPosition.x , GetMiddlePosition().y }, 15, wall)) {
					velocity.y *= -1;
					position.y = lastPosition.y - 25.0f;
				}

			}
		}
		lastPosition = GetMiddlePosition();
	}	
}


void Ball::Draw() {
	DrawTextureEx(ballTexture, { position.x, position.y }, 0, hitProgress / 1.5f, WHITE);
}
