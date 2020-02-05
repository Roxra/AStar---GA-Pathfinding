#include "Map.h"

Map::Map(int x, int y)
{
	SetSizeX(x);
	SetSizeY(y);
}


Map::~Map()
{
}

void Map::ChooseMapFile(SDL_Window** window, SDL_Renderer** renderer) //creates mapFile choice menu and lets user choose map used in CreateMap()
{
	TTF_Init();

	SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
	SDL_Rect startMapA;
	startMapA.x = 800;
	startMapA.y = 100;
	startMapA.w = 200;
	startMapA.h = 50;
	SDL_RenderFillRect(*renderer, &startMapA);

	SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
	SDL_Rect startMapB;
	startMapB.x = 800;
	startMapB.y = 200;
	startMapB.w = 200;
	startMapB.h = 50;
	SDL_RenderFillRect(*renderer, &startMapB);

	SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
	SDL_Rect startMapC;
	startMapC.x = 800;
	startMapC.y = 300;
	startMapC.w = 200;
	startMapC.h = 50;
	SDL_RenderFillRect(*renderer, &startMapC);

	TTF_Font * font = TTF_OpenFont("Roboto-Black.ttf", 25);
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(font, "MapFile A", color, 500);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(*renderer, surface);
	SDL_RenderCopy(*renderer, texture, NULL, &startMapA);

	SDL_Surface * surface1 = TTF_RenderText_Blended_Wrapped(font, "MapFile B", color, 500);
	SDL_Texture * texture1 = SDL_CreateTextureFromSurface(*renderer, surface1);
	SDL_RenderCopy(*renderer, texture1, NULL, &startMapB);

	SDL_Surface * surface3 = TTF_RenderText_Blended_Wrapped(font, "MapFile C", color, 500);
	SDL_Texture * texture3 = SDL_CreateTextureFromSurface(*renderer, surface3);
	SDL_RenderCopy(*renderer, texture3, NULL, &startMapC);

	SDL_RenderPresent(*renderer); //renders the screen

	int mX = 0; //mouse position on x axis
	int mY = 0; //mouse position on y axis
	bool loop = true;

	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			SDL_GetMouseState(&mX, &mY); //updates mouse position
			if (mX >= startMapA.x && mX <= startMapA.x + startMapA.w && mY >= startMapA.y && mY <= startMapA.y + startMapA.h) //if mouse in this area of the quit button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					bool isPlaying = true;
					loop = false;
					CreateMap("Map1ANSI.txt");
				}
			}
			if (event.type == SDL_QUIT) //if player presses quit button of window
			{
				loop = false;
			}
			if (mX >= startMapB.x && mX <= startMapB.x + startMapB.w && mY >= startMapB.y && mY <= startMapB.y + startMapB.h) //if mouse in this area of the play game button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					bool isPlaying = true;
					loop = false;
					CreateMap("Map2ANSI.txt");
				}
			}
			if (mX >= startMapC.x && mX <= startMapC.x + startMapC.w && mY >= startMapC.y && mY <= startMapC.y + startMapC.h) //if mouse in this area of the play game button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					bool isPlaying = true;
					loop = false;
					CreateMap("Map3ANSI.txt");
				}
			}
		}
	}
}

void Map::CreateMap(const char * mapName) //gets mapFile's text and edits map data from text
{
	std::ifstream mapFile;
	std::string line;
	mapFile.open(mapName);
	std::istream& mapFileStream = mapFile;

	std::string c;
	int i = 0;
	int found = 0;
	while (std::getline(mapFileStream, c, ' '))
	{
		std::cout << c;
		if (i == 0)
		{
			std::cout << " MapSizeY Found" << std::endl;
		    SetSizeY(std::stoi(c)); //turns string to int
		}
		else if (i == 1)
		{
			std::cout << " MapSizeX Found" << std::endl;
			SetSizeX(std::stoi(c)); //turns string to int
		}
		else
		{
			std::cout << " Number Found As The ";
			std::cout << found;
			std::cout << "'th Number" << std::endl;
			SetNumbers(std::stoi(c), found); //turns string to int and sets in which order the numbers are found
			found++;
		}
		i++;
	}
	TransferNumbers(); //moves numbers to be in 2d array equal to map size
	mapFile.close();
}

void Map::UpdateMap(SDL_Renderer** renderer) //updates visual representation of map
{
	for (int i = 0; i < GetSizeX(); i++)
	{
		for (int s = 0; s < GetSizeY(); s++)
		{
			if (GetNumber(i, s) == 2)
			{
				SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
			}
			else if (GetNumber(i, s) == 3)
			{
				SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
			}
			else if (GetNumber(i, s) == 1)
			{
				SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
			}
			SDL_Rect map;
			map.x = 20 + (s * 80);
			map.y = 20 + (i * 80);
			map.w = 60;
			map.h = 60;
			SDL_RenderFillRect(*renderer, &map);
		}
	}
}

void Map::TransferNumbers() //moves numbers to be in 2d array equal to map size
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int io = 0; io < sizeY; io++)
		{
			if (numbers[totalI] == 3)
			{
				mapNumbers[i][io] = 3; //endPos
				mapEndX = i;
				mapEndY = io;
			}
			if (numbers[totalI] == 2)
			{
				mapNumbers[i][io] = 2; //startPos
				mapStartX = i;
				mapStartY = io;
			}
			if (numbers[totalI] == 1)
			{
				mapNumbers[i][io] = 1; //wall
			}
			if (numbers[totalI] == 0)
			{
				mapNumbers[i][io] = 0; //empty
			}
			totalI++;
		}
	}
}