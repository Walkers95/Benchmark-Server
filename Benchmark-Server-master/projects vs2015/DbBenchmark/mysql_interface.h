#pragma once
// MySQL Include
#include <winsock.h>

#include "Library\MySql\include\mysql.h"
#include "Library\MySql\include\my_global.h"

MYSQL* connection_mysql;

void InitMySql(char* hostname, int port, char* dbName, char* user, char* password);

void DoBenchmarkMySql();