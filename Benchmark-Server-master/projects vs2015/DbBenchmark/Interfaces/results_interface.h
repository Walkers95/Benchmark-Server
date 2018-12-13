#pragma once
#include "Interfaces.h"


struct database_current_results* GetCurrentResults();
void SetCurrentResults(struct database_current_results* results);

double GetMinValueOfCurrentResults();
double GetMaxValueOfCurrentResults();

struct database_current_results* GetResultsBenchmark(int benchmarkID);
