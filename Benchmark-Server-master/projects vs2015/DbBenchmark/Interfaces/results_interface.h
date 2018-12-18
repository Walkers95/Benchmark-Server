#pragma once
#include "Interfaces.h"


struct database_current_results* GetCurrentResults();
void SetCurrentResults(struct database_current_results* results);

double GetMinValueOfCurrentResults();
double GetMaxValueOfCurrentResults();
double** GetResultsUser(int benchmarkID);
double** GetResultsUserData(int benchmarkID);
struct database_current_results* GetResults(int benchmarkID);
double GetResultsUserScore(int benchmarkID);