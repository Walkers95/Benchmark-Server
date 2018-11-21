#pragma once
// MySQL Include
#include <winsock.h>
#include <time.h>

#include "Interfaces.h"
#include "../Library\MySql\include\mysql.h"
#include "../Library\MySql\include\my_global.h"

MYSQL* connection_mysql;

LARGE_INTEGER frequency;
LARGE_INTEGER t1, t2;

int param_request_number;
int param_custom_script;
int param_ping;
char* param_script_read;
char* param_script_write;

double elapsedTime;
double pingLatency;

int InitMySql(struct database_params *db_param);

void FinishWithError();
void ClearResults();
int CallQuery(const char* query);

double **benchmark_result;
double GetMinValueOfResults(double** results);
double GetMaxValueOfResults(double** results);
double** GetResults();

int DoBenchmarkMySql();