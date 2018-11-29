#pragma once
#include "../Main/Includes.h"

// Memory manegement 
void* Malloc(size_t size);
void* Realloc(void* pointer, size_t size);

// Results
int request_number;
double GetMinValueOfResults(double** results);
double GetMaxValueOfResults(double** results);
double ** GetChartResults();
void SetResults(double **results);

float GetScore();
void SetScore(float score);

void SetRequestNumber(int number);