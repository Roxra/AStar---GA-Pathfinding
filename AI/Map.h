#pragma once
#include "SDL.h"
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class Map
{
private:
	int sizeX;
	int sizeY;
	int numbers[300];
	int mapNumbers[30][30];
	int mapStartX;
	int mapStartY;
	int mapEndX;
	int mapEndY;
	int totalI = 0;
public:
	Map(int x, int y);
	~Map();
	void ChooseMapFile(SDL_Window** window, SDL_Renderer** renderer);
	void UpdateMap(SDL_Renderer** renderer);
	void CreateMap(const char * mapName);
	int GetSizeX()
	{
		return sizeX;
	}
	int GetTotalI()
	{
		return totalI;
	}
	int GetSizeY()
	{
		return sizeY;
	}
	int GetNumber(int i, int o)
	{
		return mapNumbers[i][o];
	}
	int GetMapStartX()
	{
		return mapStartX;
	}
	int GetMapStartY()
	{
		return mapStartY;
	}
	int GetMapEndY()
	{
		return mapEndY;
	}
	int GetMapEndX()
	{
		return mapEndX;
	}
	void SetSizeX(int tempSize)
	{
		sizeX = tempSize;
	}
	void SetSizeY(int tempSize)
	{
		sizeY = tempSize;
	}
	void SetNumbers(int c, int found)
	{
		numbers[found] = c;
	}
	void TransferNumbers();
};