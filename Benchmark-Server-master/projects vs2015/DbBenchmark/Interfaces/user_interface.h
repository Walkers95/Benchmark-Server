#pragma once
#include "Interfaces.h"

int ConnectUserDatabase();
void DisconnectUserDatabase();
int LoginUser(struct database_login_params *db_login);

int UploadUserBenchmark(struct database_benchmark_params* benchmark, char* databaseType);
int DeleteUserBenchmark(int benchmarkID);
struct database_user_records* GetUserBenchmark(int benchmarkID);
struct database_user_records** GetUserBenchmarkData();
int GetUserBenchmarkCountSql();
int GetUserBenchmarkCount();
int GetUserID();