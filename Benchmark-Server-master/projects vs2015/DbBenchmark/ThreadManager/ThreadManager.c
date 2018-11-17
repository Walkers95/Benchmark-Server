#include "ThreadManager.h"

SDL_Thread *mysql_thread = NULL;
SDL_Thread *oracle_thread = NULL;

int MySqlThread(void * data)
{
	ConsoleOutput("Init database for : MySql", C_DEBUG);
	if (InitMySql(data))
	{
		DoBenchmarkMySql();
	}
	return 0;
}

int OracleThread(void * data)
{
	ConsoleOutput("Init database for : Oracle", C_DEBUG);
	InitOracle(data);
	DoBenchmarkOracle();
	return 0;
}

void StartBenchmarkThread(struct database_params *db_param, char * dbType)
{

	// Initialise
	if (dbType == "Oracle")
	{
		oracle_thread = SDL_CreateThread(OracleThread, "OracleThread", (void*)db_param);
	}
	else if (dbType == "Mysql")
	{
		mysql_thread = SDL_CreateThread(MySqlThread, "MySqlThread", (void*)db_param);
	}
}

void StartQueryThread()
{
}
