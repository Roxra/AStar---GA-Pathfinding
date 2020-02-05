#pragma once
#include "Map.h"

class Node
{
private:
	int posX;
	int posY;
	int ParentX;
	int	ParentY;
	float Cost;
public:
	Node();
	~Node();
	void CalculateCost(float HCost, float SCost)
	{
		Cost = HCost + SCost;
	}
	int GetPosX()
	{
		return posX;
	}
	int GetPosY()
	{
		return posY;
	}
	int GetParentX()
	{
		return ParentX;
	}
	int GetParentY()
	{
		return ParentY;
	}
	float GetCost()
	{
		return Cost;
	}
	void SetPosX(int tempSize)
	{
		posX = tempSize;
	}
	void SetPosY(int tempSize)
	{
		posY = tempSize;
	}
	void SetParentX(int tempSize)
	{
		ParentX = tempSize;
	}
	void SetParentY(int tempSize)
	{
		ParentY = tempSize;
	}
};