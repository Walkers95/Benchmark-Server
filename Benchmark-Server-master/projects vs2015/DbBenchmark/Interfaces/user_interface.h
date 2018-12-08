#pragma once
#include "Interfaces.h"

int ConnectUserDatabase();
void DisconnectUserDatabase();
int LoginUser(struct database_login_params *db_login);


struct database_user_records* GetUserBenchmark(int benchmarkID);


int GetUserBenchmarkCountSql();

struct database_user_records** GetUserBenchmarkData();
int GetUserBenchmarkCount();

int GetUserID();