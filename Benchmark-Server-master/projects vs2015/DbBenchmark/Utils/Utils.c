#include "Utils.h"

// Memory manegement 
void * Malloc(size_t size)
{
	void* adress = malloc(size);
	if (adress == NULL)
		exit(0);

	return adress;
}

void * Realloc(void * pointer, size_t size)
{
	void *address = NULL;
	realloc(address, size);

	if (address == NULL)
		exit(0);

	return address;
}


// Results
double **benchmark_results = NULL;
float score = 0;

double GetMinValueOfResults(double** results) {
	double min = 1.00f;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < request_number; j++)
		{
			if (results[i][j] != 0 && results[i][j] < min)
			{
				min = results[i][j];
			}
		}
	}

	return min;
}

double GetMaxValueOfResults(double** results) {
	double max = 0.0f;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < request_number; j++)
		{
			if (results[i][j] > max)
			{
				max = results[i][j];
			}
		}
	}

	return max;
}

double ** GetChartResults()
{
	return benchmark_results;
}

void SetResults(double ** results)
{
	// Init benchmark result
	benchmark_results = Malloc(sizeof(double*) * 2);
	for (int i = 0; i < 2; i++)
	{
		benchmark_results[i] = Malloc(sizeof(double) * request_number);
	}

	benchmark_results = results;
}

float GetScore()
{
	return score;
}

void SetScore(float _score)
{
	score = _score;
}

void SetRequestNumber(int number)
{
	request_number = number;
}

