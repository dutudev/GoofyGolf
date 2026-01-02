#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "raylib.h"
#include "map.hpp"

using std::string;
using std::vector;

Vector2 Map::GetBallStartPos() {
	return {ballStartPosition.x * 50.0f, ballStartPosition.y * 50.0f };
}

Vector2 Map::GetHolePos() {
	return { holePosition.x * 50.0f + 25.0f, holePosition.y * 50.0f + 25.0f };
}

bool Map::LoadMap(string filePath, vector<Rectangle>& walls) {
	if (walls.size() > 0) {
		walls.clear();
	}

	filePath = (std::string)GetWorkingDirectory() + "/assets/maps/" + filePath + ".csv";
	std::fstream mapFile;
	
	mapFile.open(filePath, std::fstream::in);
	if (mapFile.is_open()) {
		for (int i = 0; i < 12; i++) {
			int j = 0;
			string currentNumberStr;
			while (getline(mapFile, currentNumberStr)) {
				int numberCur = 0;
				for (int y = 0; y < currentNumberStr.length(); y++) {
					if (currentNumberStr[y] != ',' || y == currentNumberStr.length() - 1) {
						numberCur *= 10;
						numberCur += currentNumberStr[y] - '0';
						//std::cout << numberCur << '\n';
					}
					else {
						//std::cout << numberCur << '\n';
						if (numberCur == 15) {
							holePosition = { (float)j, (float)i };
						}
						else if(numberCur == 20) {
							ballStartPosition = { (float)j, (float)i };
							//std::cout << "GURT YO" << '\n';
							numberCur = 0;
						}
						
						if ((numberCur >= 1 && numberCur <= 13) || (numberCur >= 21 && numberCur <=22)) {
							walls.push_back(Rectangle{ j * 50.0f, i * 50.0f, 50, 50 });
						}
						if (numberCur > 20) {
							numberCur--;
						}
						mapData[j][i] = numberCur;
						//std::cout << mapData[j][i] << ' ' << j << ' ' << i << '\n';
						numberCur = 0;
						j++;
					}
				}
				if (numberCur == 15) {
					holePosition = { (float)j, (float)i };
				}
				else if (numberCur == 20) {
					ballStartPosition = { (float)j, (float)i };
					//std::cout << "GURT YO" << '\n';
					numberCur = 0;
				}
				
				if ((numberCur >= 1 && numberCur <= 13) || (numberCur >= 21 && numberCur <= 22)){
					walls.push_back(Rectangle{ j * 50.0f, i * 50.0f, 50, 50 });
				}
				if (numberCur > 20) {
					numberCur--;
				}
				mapData[j][i] = numberCur;
				//std::cout << mapData[j][i] << ' ' << j << ' ' << i << '\n';
				j = 0;
				i++;
				/*
				mapData[j][i] = stoi(currentNumberStr);
				std::cout << mapData[j][i] << ' ' << j << ' ' << i << '\n';
				j++;
				if (j >= 16) {
					j = 0;
					i++;
				}*/
			}
		}
		mapFile.close();
		mapLoaded = true;
		std::cout << "DEBUG: Map Loaded\n";
		return true;
	}
	else {
		//std::cout << "No map available\n";
		mapFile.close();
		return false;
	}
}

void Map::DrawMap(const Texture& mapTexture) {
	if (mapLoaded) {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 12; j++) {
				Rectangle rect = { mapData[i][j] * 50, 0, 50, 50 };
				DrawTextureRec(mapTexture, rect, { i * 50.0f, j * 50.0f }, WHITE);
			}
		}
	}
}