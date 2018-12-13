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
	char* hostname;
	int port;
	char* database;
	char* user;
	char* password;
	int pingCompensation;
	int request_number;
	int custom_script;
	char* script_read;
	char* scrit_write;
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