#pragma once
class Chromosome
{
private:
	float fitness;
	float chanceOfPick;
	int genes[500];

public:
	Chromosome();
	~Chromosome();

	void GenerateChromosome(int cN);

	void Mutation(int cN);

	void SetNumberSet(int tempNumberSet, int geneNumber)
	{
		genes[geneNumber] = tempNumberSet;
	}
	int GetNumberSet(int geneNumber)
	{
		return genes[geneNumber];
	}
	void SetFitness(float tempFitness)
	{
		fitness = tempFitness;
	}
	float GetFitness()
	{
		return fitness;
	}
	void SetChanceOfPick(float tempChanceOfPick)
	{
		chanceOfPick = tempChanceOfPick;
	}
	float GetChanceOfPick()
	{
		return chanceOfPick;
	}
};