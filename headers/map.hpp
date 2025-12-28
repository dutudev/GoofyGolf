#include <string>
#include "raylib.h"
#pragma once

using std::string;
class Map {
private:
	bool mapLoaded = false;
	int mapData[16][12];
	Vector2 holePosition, ballStartPosition;
public:
	Vector2 GetBallStartPos();
	bool LoadMap(string filePath);
	void DrawMap(const Texture& mapTexture);
};