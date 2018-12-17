#include "results_interface.h"

struct database_current_results* current_results = NULL;

struct database_current_results * GetCurrentResults()
{
	return current_results;
}

void SetCurrentResults(struct database_current_results * results)
{
	current_results = results;

	current_results->minValue = GetMinValueOfCurrentResults();
	current_results->maxValue = GetMaxValueOfCurrentResults();
}


double GetResultsUserScore(int benchmarkID)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT score FROM results WHERE benchmark_id = %d", benchmarkID);

	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);

	if (results == NULL)
		return 0;

	double scoreReturn = 0.00f;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		scoreReturn = atof(row[0]);
	}

	ClearMySqlResults();
	DisconnectUserDatabase();

	return scoreReturn;
}

double **GetResultsUser(int benchmarkID)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT results FROM results WHERE benchmark_id = %d", benchmarkID);

	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);

	if (results == NULL)
		return 0;

	char* returnJson = NULL;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		returnJson = Malloc(strlen(row[0]) + 1);
		returnJson = row[0];
	}

	ClearMySqlResults();
	DisconnectUserDatabase();

	double** jsonResult = GetJsonBenchmarkResults(returnJson);

	return jsonResult;
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

struct database_current_results * GetResults(int benchmarkID)
{

	return NULL;
}
