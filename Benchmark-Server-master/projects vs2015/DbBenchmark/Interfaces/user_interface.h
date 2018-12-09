#pragma once
#include "Interfaces.h"

int ConnectUserDatabase();
void DisconnectUserDatabase();
int LoginUser(struct database_login_params *db_login);

int DeleteUserBenchmark(int benchmarkID);
double** GetUserBenchmarkResults(int benchmarkID);
struct database_user_records* GetUserBenchmark(int benchmarkID);
struct database_user_records** GetUserBenchmarkData();
int GetUserBenchmarkCountSql();

int GetUserBenchmarkCount();
int GetUserID();