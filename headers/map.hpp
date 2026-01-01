#include <string>
#include <vector>
#include "raylib.h"
#pragma once

using std::string;
using std::vector;
class Map {
private:
	bool mapLoaded = false;
	int mapData[16][12];
	Vector2 holePosition, ballStartPosition;
public:
	Vector2 GetBallStartPos();
	bool LoadMap(string filePath, vector<Rectangle>& walls);
	void DrawMap(const Texture& mapTexture);
};