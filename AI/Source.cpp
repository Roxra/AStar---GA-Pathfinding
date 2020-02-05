#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "Map.h"
#include "Chromosome.h"
#include "Node.h"
#include "SDL.h"
#include <SDL_ttf.h>

void GeneticAlgorithm(Map &map, bool &isPlaying, SDL_Renderer** renderer);
bool checkCollisions(Map &map, int currentPosX, int currentPosY, bool &isPlaying);
Chromosome Roulette(Chromosome chromosomes[], int cN, int n);
void UpdatePath(SDL_Renderer** renderer, int currentPosX, int currentPosY, int colour);
void Crossover(Chromosome &firstFittest, Chromosome &secondFittest, int cN);
void APathfinding(Map &map, bool &isPlaying, SDL_Renderer** renderer);
void MoveToOpenList(Map map, Node &tempNode, Node &currentNode, Node &startNode, std::vector<Node> &openList, std::vector<Node> closedList, SDL_Renderer** renderer, int numberOfMoves);
void MainMenu(SDL_Window** window, SDL_Renderer**);
void readySDL(SDL_Window** window, SDL_Renderer** renderer);

inline bool operator < (Node &a, Node &b)
{
	return a.GetCost() < b.GetCost(); //checks which has higher cost 
}

int main(int argc, char* argv[])
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	readySDL(&window, &renderer);
	MainMenu(&window, &renderer);
	return 0;
}

void GeneticAlgorithm(Map &map, bool &isPlaying, SDL_Renderer** renderer)
{
	int const n = 12; //number of chromosomes
	int cN = map.GetTotalI() * 2; //length of chromosome scales with map size
	Chromosome chromosomes[n];
	int numberOfGenerations = 0;

	for (int i = 0 ; i < n ; i++ )
	{
		chromosomes[i].GenerateChromosome(cN); //for every chromosome, create random genes
	}

	SDL_SetRenderDrawColor(*renderer, 116, 184, 242, 255);
	SDL_Rect background;
	background.x = 0;
	background.y = 0;
	background.w = 5000;
	background.h = 5000;
	SDL_RenderFillRect(*renderer, &background);
	map.UpdateMap(renderer);

	SDL_RenderPresent(*renderer);

	int start_s = clock();

	while (isPlaying)
	{
		int kno = 0;
		numberOfGenerations++;
		while (kno < n)
		{
			int currentPosX = map.GetMapStartX();
			int currentPosY = map.GetMapStartY();
			int cPos = 0;
			std::cout << "CHROMOSOME NUMBER: " << kno << std::endl << std::endl << std::endl;
			map.UpdateMap(renderer);
			int distance = 0;
			while (cPos <= cN && isPlaying)
			{
				if (chromosomes[kno].GetNumberSet(cPos) == 1 && chromosomes[kno].GetNumberSet(cPos + 1) == 1) //move up
				{
					currentPosY = currentPosY--;
					currentPosX = currentPosX;

					if (checkCollisions(map, currentPosX, currentPosY, isPlaying))
					{
						std::cout << "Path Found" << std::endl;
						distance++;
					}
					else
					{
						currentPosY = currentPosY++; //reverts position if not valid
						currentPosX = currentPosX;
					}
				}
				else if (chromosomes[kno].GetNumberSet(cPos) == 0 && chromosomes[kno].GetNumberSet(cPos + 1) == 1) //move right
				{
					currentPosY = currentPosY;
					currentPosX = currentPosX++;

					if (checkCollisions(map, currentPosX, currentPosY, isPlaying))
					{
						std::cout << "Path Found" << std::endl;
						distance++;
					}
					else
					{
						currentPosY = currentPosY; //reverts position if not valid
						currentPosX = currentPosX--;
					}
				}
				else if (chromosomes[kno].GetNumberSet(cPos) == 1 && chromosomes[kno].GetNumberSet(cPos + 1) == 0) //move left
				{
					currentPosY = currentPosY;
					currentPosX = currentPosX--;

					if (checkCollisions(map, currentPosX, currentPosY, isPlaying))
					{
						std::cout << "Path Found" << std::endl;
						distance++;
					}
					else
					{
						currentPosY = currentPosY; //reverts position if not valid
						currentPosX = currentPosX++;
					}
				}
				else if (chromosomes[kno].GetNumberSet(cPos) == 0 && chromosomes[kno].GetNumberSet(cPos + 1) == 0) //move down
				{
					currentPosY = currentPosY++;
					currentPosX = currentPosX;

					if (checkCollisions(map, currentPosX, currentPosY, isPlaying))
					{
						std::cout << "Path Found" << std::endl;
						distance++;
					}
					else
					{
						currentPosY = currentPosY--; //reverts position if not valid
						currentPosX = currentPosX;
					}
				}
				UpdatePath(renderer, currentPosY, currentPosX, 0); //updates sdl render
				cPos++;
				cPos++;
			}

			float dX = std::abs(map.GetMapEndX() - currentPosX);
			float dY = std::abs(map.GetMapEndY() - currentPosY);
			float result = 1 / (dX + dY + 1 + distance); //fitness function
			chromosomes[kno].SetFitness(result);
			std::cout << "   " << kno << " result is: " << result << std::endl;
			kno++;
		}

		Chromosome chromosomeFitList[n]; //temp store of new chromosomes for crossover

		for (int i = 0; i < n; i++) //roulette function and crossover
		{
			Chromosome firstFittest = Roulette(chromosomes,cN,n);
			Chromosome secondFittest = Roulette(chromosomes,cN,n);

			Crossover(firstFittest, secondFittest, cN); //chance of crossing over first fittest and second fittest

			chromosomeFitList[i] = firstFittest; //stores firstfittest and secondfittest to temp array
			chromosomeFitList[i + 1] = secondFittest;
			i++;
		}

		for (int i = 0; i < n; i++) //edits orignal chromosomes to be equal to new temp ones and then carries out mutation
		{
			chromosomes[i] = chromosomeFitList[i];
			chromosomes[i + 1] = chromosomeFitList[i + 1];
			chromosomes[i].Mutation(cN);
			chromosomes[i + 1].Mutation(cN);
			i++;;
		}
	}

	int stop_s = clock();
	std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << std::endl;
	SDL_Delay(6000);
}

bool checkCollisions(Map &map, int currentPosX, int currentPosY, bool &isPlaying) //checks if currentPos is in a wall,outside of boundaries or has reached the goal. used for both algorithms
{
	if (currentPosX < 0 || currentPosY < 0 || currentPosX > map.GetSizeX() - 1 || currentPosY > map.GetSizeY() - 1)
	{
		std::cout << " OUTSIDE POS " << std::endl;
		return false;
	}
	else if (map.GetNumber(currentPosX, currentPosY) == 1)
	{
		std::cout << " IN WALL " << std::endl;
		return false;
	}
	else if (currentPosY == map.GetMapEndY() && currentPosX == map.GetMapEndX())
	{
		std::cout << " IT WORKED " << std::endl;
		isPlaying = false;
		return true;
	}
	else
	{
		std::cout << " MOVE TO NEXT POS " << std::endl;
		return true;
	}
}

Chromosome Roulette(Chromosome chromosomes[], int cN, int n)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(0, 1);
	float r = distribution(generator);
	float prevChance = 0;
	float totalChance = 0;

	float totalFit = 0;

	for (int i = 0; i < n; i++)
	{
		totalFit = totalFit + chromosomes[i].GetFitness();
	}

	for (int i = 0; i < n; i++)
	{
		chromosomes[i].SetChanceOfPick(chromosomes[i].GetFitness() / totalFit);
	}

	for (int i = 0; i < cN; i++) //for each chromosome see if random float picks it on roulette
	{
		totalChance += chromosomes[i].GetChanceOfPick();
		if (r < totalChance && r > prevChance) //if random float is between totalChance and prevChance (previous totalChance)
		{
			return chromosomes[i];
		}
		prevChance = totalChance;
	}
}

void Crossover(Chromosome &firstFittest, Chromosome &secondFittest, int cN)
{
	float crossoverThreshold = cN / 2;

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(0, 1);
	float r = distribution(generator);

	if (r <= 0.6f)
	{
		for (int i = 0; i < crossoverThreshold; i++) //swaps numbers of first half of each chromsome
		{
			int currentFirst = firstFittest.GetNumberSet(i);
			int currentSecond = secondFittest.GetNumberSet(i);
			std::swap(currentFirst, currentSecond);
			firstFittest.SetNumberSet(currentFirst,i);
			secondFittest.SetNumberSet(currentSecond,i);
		}
		for (int i = 0; i < cN; i++)
		{
			std::cout << firstFittest.GetNumberSet(i);
		}
	}
}

void UpdatePath(SDL_Renderer** renderer, int currentPosX, int currentPosY, int colour)
{
	SDL_SetRenderDrawColor(*renderer, 0, 255, colour, 255);
	SDL_Rect mapRect;
	mapRect.x = 20 + (currentPosX * 80);
	mapRect.y = 20 + (currentPosY * 80);
	mapRect.w = 60;
	mapRect.h = 60;
	SDL_RenderFillRect(*renderer, &mapRect);
	SDL_RenderPresent(*renderer);
}

void APathfinding(Map &map, bool &isPlaying, SDL_Renderer** renderer)
{
	Node currentNode;
	currentNode.SetPosX(map.GetMapStartX());
	currentNode.SetPosY(map.GetMapStartY());
	Node startNode;
	startNode.SetPosX(map.GetMapStartX());
	startNode.SetPosY(map.GetMapStartY());
	Node endNode;
	endNode.SetPosX(map.GetMapEndX());
	endNode.SetPosY(map.GetMapEndY());
	std::vector<Node> openList;
	std::vector<Node> closedList;

	SDL_SetRenderDrawColor(*renderer, 116, 164, 242, 255);
	SDL_Rect background;
	background.x = 0;
	background.y = 0;
	background.w = 1920;
	background.h = 1080;
	SDL_RenderFillRect(*renderer, &background);

	SDL_RenderPresent(*renderer);

	int start_s = clock();

	openList.push_back(currentNode);
	bool endFound = false;
	int numberOfMoves = 0;
	std::vector<Node> endList;

	map.UpdateMap(renderer);

	while (isPlaying)
	{
		if (endFound)
		{
			if (currentNode.GetPosX() == startNode.GetPosX() && currentNode.GetPosY() == startNode.GetPosY()) //checks if currentNode is at start
			{
				UpdatePath(renderer, currentNode.GetPosY(), currentNode.GetPosX(), 255); //updates SDL render
				std::cout << "Success!! " << std::endl;
				int stop_s = clock();
				std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << std::endl;
				for (Node node : endList)
				{
					std::cout << node.GetPosX() << node.GetPosY() << " ";
				}
				SDL_Delay(5000);
				isPlaying = false;
			}
			if (!closedList.empty())
			{
				UpdatePath(renderer, currentNode.GetPosY(), currentNode.GetPosX(), 255); //updates SDL render
				endList.push_back(currentNode);
				currentNode.SetPosX(currentNode.GetParentX());
				currentNode.SetPosY(currentNode.GetParentY());
				for (size_t i = 0; i < closedList.size(); i++)
				{
					if (currentNode.GetPosX() == closedList.at(i).GetPosX() && currentNode.GetPosY() == closedList.at(i).GetPosY()) //finds node with equal position to currentNode (to get parent of previous node)
					{
						currentNode = closedList.at(i);
					}
				}
			}			
		}

		if (!openList.empty() && endFound == false)
		{
			std::sort(openList.begin(), openList.end()); //sorts openList by lowest cost
			currentNode = openList.front(); //assign current node from lowest cost in open list
			UpdatePath(renderer, currentNode.GetPosY(), currentNode.GetPosX(), 0); //updates SDL render
			if (currentNode.GetPosX() == endNode.GetPosX() && currentNode.GetPosY() == endNode.GetPosY()) //if end reached
			{
				endFound = true;
				endList.push_back(closedList.back());
				currentNode = closedList.back();
			}
			else
			{
				closedList.push_back(currentNode); //adds currentNode to closedList
				Node tempNode;
				for (int i = 0; i < 4; i++) //examine each adjecent node
				{
					tempNode = currentNode;
					switch (i)
					{
					case 0:
						tempNode.SetPosX(currentNode.GetPosX() - 1); 
						MoveToOpenList(map,tempNode,currentNode, startNode, openList, closedList, renderer, numberOfMoves + 1);
						break;
					case 1:
						tempNode.SetPosX(currentNode.GetPosX() + 1);
						MoveToOpenList(map, tempNode, currentNode, startNode, openList, closedList, renderer, numberOfMoves + 1);
						break;
					case 2:
						tempNode.SetPosY(currentNode.GetPosY() - 1);
						MoveToOpenList(map, tempNode, currentNode, startNode, openList, closedList, renderer, numberOfMoves + 1);
						break;
					case 3:
						tempNode.SetPosY(currentNode.GetPosY() + 1);
						MoveToOpenList(map, tempNode, currentNode, startNode, openList, closedList, renderer, numberOfMoves + 1);
						break;
					default:
						break;
					}
				}
				openList.erase(openList.begin());
			}
		}
	}
}

void MoveToOpenList(Map map,Node &tempNode, Node &currentNode, Node &startNode, std::vector<Node> &openList , std::vector<Node> closedList, SDL_Renderer** renderer, int numberOfMoves)
{
	bool Check;
	bool isFound = false;
	if (checkCollisions(map,tempNode.GetPosX(),tempNode.GetPosY(), Check)) //checks if nodePos is valid
	{
		for( Node node : closedList) //checks if node is already in closedlist
		{
			if( node.GetPosX() == tempNode.GetPosX() && node.GetPosY() == tempNode.GetPosY())
			{
				isFound = true;
			}
			else {}
		}
		for (Node node : openList) //checks if node is already in openlist
		{
			if (node.GetPosX() == tempNode.GetPosX() && node.GetPosY() == tempNode.GetPosY())
			{
				isFound = true;
			}
			else {}
		}
		if (isFound == false)
		{
			int HCost = std::abs(tempNode.GetPosX() - map.GetMapEndX()) + std::abs(tempNode.GetPosY() - map.GetMapEndY()); //Hcost function
			tempNode.CalculateCost(HCost, numberOfMoves); //totalcost function
			std::cout << tempNode.GetCost() << std::endl;
			tempNode.SetParentX(currentNode.GetPosX());
			tempNode.SetParentY(currentNode.GetPosY());
			openList.push_back(tempNode); //adds tempNode to openList
			numberOfMoves++;
			//SDL_Delay(60);
			std::cout << "Moving " << tempNode.GetPosX() << tempNode.GetPosY() << " to open list" << std::endl;
		}
	}
}

void MainMenu(SDL_Window** window, SDL_Renderer** renderer)
{
	SDL_SetRenderDrawColor(*renderer, 30, 30, 30, 255);
	SDL_Rect backgroundRect;
	backgroundRect.x = 0;
	backgroundRect.y = 0;
	backgroundRect.w = 1920;
	backgroundRect.h = 1080;
	SDL_RenderFillRect(*renderer, &backgroundRect);

	SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
	SDL_Rect startGeneticRect;
	startGeneticRect.x = 400;
	startGeneticRect.y = 100;
	startGeneticRect.w = 200;
	startGeneticRect.h = 50;
	SDL_RenderFillRect(*renderer, &startGeneticRect);

	SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
	SDL_Rect startAPathfindingRect;
	startAPathfindingRect.x = 400;
	startAPathfindingRect.y = 200;
	startAPathfindingRect.w = 200;
	startAPathfindingRect.h = 50;
	SDL_RenderFillRect(*renderer, &startAPathfindingRect);

	SDL_SetRenderDrawColor(*renderer, 0, 255, 0, 255);
	SDL_Rect guideRect;
	guideRect.x = 400;
	guideRect.y = 300;
	guideRect.w = 200;
	guideRect.h = 50;
	SDL_RenderFillRect(*renderer, &guideRect);

	SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
	SDL_Rect quitRect;
	quitRect.x = 400;
	quitRect.y = 400;
	quitRect.w = 200;
	quitRect.h = 50;
	SDL_RenderFillRect(*renderer, &quitRect);

	TTF_Font * font = TTF_OpenFont("Roboto-Black.ttf", 25);
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(font, "Genetic Algorithm", color,500);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(*renderer, surface);
	SDL_RenderCopy(*renderer, texture, NULL, &startGeneticRect);

	SDL_Surface * surface1 = TTF_RenderText_Blended_Wrapped(font, "A* Pathfinding", color,500);
	SDL_Texture * texture1 = SDL_CreateTextureFromSurface(*renderer, surface1);
	SDL_RenderCopy(*renderer, texture1, NULL, &startAPathfindingRect);

	SDL_Surface * surface2 = TTF_RenderText_Blended_Wrapped(font, "Instructions", color,500);
	SDL_Texture * texture2 = SDL_CreateTextureFromSurface(*renderer, surface2);
	SDL_RenderCopy(*renderer, texture2, NULL, &guideRect);

	SDL_Surface * surface3 = TTF_RenderText_Blended_Wrapped(font, "Quit Game", color,500);
	SDL_Texture * texture3 = SDL_CreateTextureFromSurface(*renderer, surface3);
	SDL_RenderCopy(*renderer, texture3, NULL, &quitRect);

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
			if (mX >= quitRect.x && mX <= quitRect.x + quitRect.w && mY >= quitRect.y && mY <= quitRect.y + quitRect.h) //if mouse in this area of the quit button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					loop = false;
				}
			}
			if (event.type == SDL_QUIT) //if player presses quit button of window
			{
				loop = false;
			}
			if (mX >= startGeneticRect.x && mX <= startGeneticRect.x + startGeneticRect.w && mY >= startGeneticRect.y && mY <= startGeneticRect.y + startGeneticRect.h) //if mouse in this area of the play game button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					bool isPlaying = true;
					loop = false;
					Map* currentMap = new Map(10, 10);
					currentMap->ChooseMapFile(window, renderer);
					GeneticAlgorithm(*currentMap, isPlaying, renderer);
				}
			}
			if (mX >= startAPathfindingRect.x && mX <= startAPathfindingRect.x + startAPathfindingRect.w && mY >= startAPathfindingRect.y && mY <= startAPathfindingRect.y + startAPathfindingRect.h) //if mouse in this area of the play game button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					bool isPlaying = true;
					loop = false;
					Map* currentMap = new Map(10, 10);
					currentMap->ChooseMapFile(window, renderer);
					currentMap->UpdateMap(renderer);
					APathfinding(*currentMap, isPlaying, renderer);
				}
			}
			if (mX >= guideRect.x && mX <= guideRect.x + guideRect.w && mY >= guideRect.y && mY <= guideRect.y + guideRect.h) //if mouse in this area of the instructions button
			{
				if (event.type == SDL_MOUSEBUTTONDOWN) //creates 2 objects with text
				{
					SDL_Surface * surface2 = TTF_RenderText_Blended_Wrapped(font, "Place a mapfile in the file directory with the correct name and decide if you would like A* Pathfinding or Genetic Algorithm", color , 900);
					SDL_Texture * texture2 = SDL_CreateTextureFromSurface(*renderer, surface2);
					int texW = 0;
					int texH = 0;
					SDL_QueryTexture(texture2, NULL, NULL, &texW, &texH);
					SDL_Rect dstrect = { 50, 550, texW, texH };
					SDL_RenderCopy(*renderer, texture2, NULL, &dstrect);
					SDL_RenderPresent(*renderer);
				}
			}
		}
	}
}

void readySDL(SDL_Window** window, SDL_Renderer** renderer)
{
	*window = SDL_CreateWindow("Pathfinding AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 900, SDL_WINDOW_RESIZABLE);
	TTF_Init(); //initilises text library of SDL
	SDL_Init(SDL_INIT_AUDIO);

	if (*window == nullptr)
	{
		std::cout << "Failed to create window!!! :( " << SDL_GetError();
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

	if (*renderer == nullptr)
	{
		std::cout << "Could not create renderer!!! :( " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(*window);
	}
}