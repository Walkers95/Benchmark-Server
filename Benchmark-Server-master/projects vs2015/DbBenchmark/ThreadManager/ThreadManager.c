#include "ThreadManager.h"

SDL_Thread *mysql_thread = NULL;
SDL_Thread *oracle_thread = NULL;

int MySqlThread(void * data)
{
	ConsoleOutput("Init database for : MySql", C_DEBUG);
	if (InitMySql((struct database_params*)data))
	{
		if (!DoBenchmarkMySql())
		{
			ConsoleOutput("Failed to run benchmark !", C_ERROR);
		}
		else
		{
			ConsoleOutput("Benchmark successfull !", C_SUCCESS);
		}
			
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
		oracle_thread = SDL_CreateThread(OracleThread, "OracleThread", db_param);
	}
	else if (dbType == "Mysql")
	{
		mysql_thread = SDL_CreateThread(MySqlThread, "MySqlThread", db_param);
	}
}

void StartQueryThread()
{
}
