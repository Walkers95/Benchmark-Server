#pragma once
#include "../Interfaces/Interfaces.h"
#include "../Library/cJSON/cJSON.h"

char* FormatJsonBenchmarkResults(struct database_current_results *results);
double* GetJsonBenchmarkResultsFromFile(char* file);
void SaveJsonBenchmarkResults(char* file);
double **GetJsonBenchmarkResults(char *json);
char* FormatJsonBenchmarkResults(struct database_current_results *results);