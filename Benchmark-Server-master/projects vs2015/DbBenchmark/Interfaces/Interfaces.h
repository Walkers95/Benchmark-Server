#pragma once
#include "mysql_interface.h"
#include "oracle_interface.h"

#include "../Console/Console.h"

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
