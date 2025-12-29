#pragma once
#include <string>
#include "raylib.h"

class Ball {
private:
	const Vector2 size = {30, 30};
	Vector2 position = { 0, 0 };
	Vector2 velocity = { 0, 0 };
	Texture ballTexture;
	//add something for decel maybe
public:
	Ball();
	~Ball();
	Vector2 GetPosition();
	Vector2 GetMiddlePosition();
	void SetPosition(Vector2 posToSet);
	void SetVelocity(Vector2 velToSet);
	void Logic();
	void Draw();
};