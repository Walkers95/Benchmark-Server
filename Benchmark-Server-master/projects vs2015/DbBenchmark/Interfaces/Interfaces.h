#pragma once
#include "mysql_interface.h"
#include "oracle_interface.h"


struct database_params
{
	char* hostname;
	int port;
	char* dbName;
	char* user;
	char* password;
};
