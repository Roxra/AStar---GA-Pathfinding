#include "Chromosome.h"
#include <random>

Chromosome::Chromosome()
{
}


Chromosome::~Chromosome()
{
}

void Chromosome::GenerateChromosome(int cN)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(0, 1);

	for (int j = 0; j < cN; j++)
	{
		float r = distribution(generator);
		if (r <= 0.5f)
		{
			SetNumberSet(0, j);
		}
		else
		{
			SetNumberSet(1, j);
		}
	}
}

void Chromosome::Mutation(int cN)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(0, 1);

	for (int io = 0; io < cN; io++)
	{
		float r = distribution(generator);

		if (r <= 0.1f)
		{
			if (GetNumberSet(io) == 1)
			{
				SetNumberSet(0, io);
			}
			else if (GetNumberSet(io) == 0)
			{
				SetNumberSet(1, io);
			}
		}
		else
		{
			//don't copy :)
		}
	}
}
