#pragma once
// MySQL Include
#include <winsock.h>
#include <time.h>

#include "Library\MySql\include\mysql.h"
#include "Library\MySql\include\my_global.h"

#define REQUEST_COUNT 50

MYSQL* connection_mysql;

LARGE_INTEGER frequency;
LARGE_INTEGER t1, t2;
double elapsedTime;

void InitMySql(char* hostname, int port, char* dbName, char* user, char* password);

void FinishWithError();
int CallQuery(const char* query);
char* GetResult();

void DoBenchmarkMySql();