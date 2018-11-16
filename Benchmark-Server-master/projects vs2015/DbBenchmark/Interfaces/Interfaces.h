#pragma once
#include "mysql_interface.h"
#include "oracle_interface.h"

#include "../Console/Console.h"

struct database_params
{
	char* hostname;
	int port;
	char* user;
	char* password;
};
