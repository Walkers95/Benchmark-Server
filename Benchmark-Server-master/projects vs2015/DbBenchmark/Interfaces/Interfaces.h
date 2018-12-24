#pragma once
#include "mysql_interface.h"
#include "oracle_interface.h"
#include "user_interface.h"
#include "results_interface.h"

#include "../Console/Console.h"
#include "../Utils/jSONManager.h"

struct database_login_params
{
	char* user;
	char* password;
};

struct database_benchmark_params
{
	char hostname[64];
	int port;
	char database[64];
	char user[64];
	char password[64];
	int pingCompensation;
	int request_number;
	int custom_script;
	char script_read[512];
	char scrit_write[512];
	int multi_threads;
};

struct database_user_records
{
	int id;
	char* date;
	char* databaseType;
	struct database_benchmark_params* db_param;
};

struct database_current_results
{
	int request_number;
	double score;
	double **results;
	double minValue;
	double maxValue;
};