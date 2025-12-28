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
	void SetPosition(Vector2 posToSet);
	void Draw();
};