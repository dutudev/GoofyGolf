#pragma once
#include <string>
#include <vector>
#include "raylib.h"

using std::vector;

class Ball {
private:
	const float decedeceleration = 565.0f;
	const Vector2 size = {30, 30};
	
	bool hitHole = false;
	float hitProgress = 1.5f;
	Vector2 position = { 0, 0 };
	Vector2 velocity = { 0, 0 };
	Vector2 hitPosition = { 0, 0 };
	Texture ballTexture;
	//add something for decel maybe
public:
	Ball();
	~Ball();
	Vector2 GetPosition();
	Vector2 GetMiddlePosition();
	bool GetHitHole();
	bool IsMoving();
	void SetPosition(Vector2 posToSet);
	void SetVelocity(Vector2 velToSet);
	void Logic(vector<Rectangle>& mapWalls, Vector2& holePos);
	void Draw();
};