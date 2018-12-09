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

double elapsedTime;
double pingLatency;

int InitMySql(struct database_benchmark_params *db_param);

void FinishWithError();
void ClearMySqlResults();
int CallQuery(const char* query);

int DoBenchmarkMySql(struct database_benchmark_params *db_param);



