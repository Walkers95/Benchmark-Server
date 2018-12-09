#include "results_interface.h"

struct database_current_results* current_results = NULL;

struct database_current_results * GetCurrentResults()
{
	return current_results;
}

void SetCurrentResults(struct database_current_results * results)
{
	current_results = Malloc(sizeof(struct database_current_results));

	current_results->results = Malloc(sizeof(double*) * 2);
	for (int i = 0; i < 2; i++)
	{
		current_results->results[i] = Malloc(sizeof(double) * current_results->request_number);
	}
	current_results = results;

	current_results->minValue = GetMinValueOfCurrentResults();
	current_results->maxValue = GetMaxValueOfCurrentResults();
}


double GetMinValueOfCurrentResults() {
	double min = 1.00f;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < current_results->request_number; j++)
		{
			if (current_results->results[i][j] != 0 && current_results->results[i][j] < min)
			{
				min = current_results->results[i][j];
			}
		}
	}

	return min;
}

double GetMaxValueOfCurrentResults() {
	double max = 0.0f;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < current_results->request_number; j++)
		{
			if (current_results->results[i][j] > max)
			{
				max = current_results->results[i][j];
			}
		}
	}

	return max;
}

struct database_current_results * GetResultsBenchmark(int benchmarkID)
{

	return NULL;
}
