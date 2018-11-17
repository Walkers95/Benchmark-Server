#include "mysql_interface.h"

double **benchmark_result;

int InitMySql(struct database_params *db_param)
{
	connection_mysql = mysql_init(NULL);

	if (connection_mysql == NULL)
	{
		ConsoleOutput(mysql_error(connection_mysql), C_ERROR);
		return 0;
	}

	if (db_param->hostname == "localhost")
	{
		if (mysql_real_connect(connection_mysql, db_param->hostname, db_param->user, db_param->password, NULL, 0, NULL, 0) == NULL)
		{
			ConsoleOutput(mysql_error(connection_mysql), C_ERROR);
			mysql_close(connection_mysql);
			return 0;
		}
	} 
	else
	{
		if (mysql_real_connect(connection_mysql, db_param->hostname, db_param->user, db_param->password, NULL, db_param->port, NULL, 0) == NULL)
		{
			ConsoleOutput(mysql_error(connection_mysql), C_ERROR);
			mysql_close(connection_mysql);
			return 0;
		}
	}

	return 1;
}

void FinishWithError()
{
	ConsoleOutput(mysql_error(connection_mysql), C_ERROR);
	mysql_close(connection_mysql);
	fgets(NULL, 0, stdin);
}

int CallQuery(const char * query)
{
	if (mysql_query(connection_mysql, query)) {
		FinishWithError();
	}
}

double ** GetResult()
{
	return benchmark_result;
}


void DoBenchmarkMySql()
{
	// On DROP la database testdb au cas ou elle existe déja
	ConsoleOutput("Making sure testdb does not exist !", C_DEBUG);
	CallQuery("DROP DATABASE testdb");
	ConsoleOutput("Success !", C_SUCCESS);
	
	// on crée notre database de test
	ConsoleOutput("Creating test database", C_DEBUG);
	CallQuery("CREATE DATABASE testdb");
	ConsoleOutput("Success !", C_SUCCESS);

	// On selectionne la db crée
	ConsoleOutput("Selection de la base", C_DEBUG);
	CallQuery("USE testdb");
	ConsoleOutput("Success !", C_SUCCESS);

	// Create table 
	ConsoleOutput("Creating test table", C_DEBUG);
	CallQuery("CREATE TABLE testtable(id INTEGER AUTO_INCREMENT PRIMARY KEY,int_test INTEGER,text_test TEXT)");
	ConsoleOutput("Success !", C_SUCCESS);

	

	// WRITE BENCHMARK
	ConsoleOutput("Starting Write Benchmark", C_DEBUG);
	for (int i = 0; i < REQUEST_COUNT; i++)
	{
		QueryPerformanceFrequency(&frequency);
		// GET TIME BEFORE
		QueryPerformanceCounter(&t1);
		CallQuery("INSERT INTO testtable(int_test, text_test) VALUES(1,'bonjour')");
		// GET TIME AFTER
		QueryPerformanceCounter(&t2);
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		ConsoleOutputValue("Write", elapsedTime);

		// Save time 
		//benchmark_result[0][i] = elapsedTime;
	}
	ConsoleOutput("Success !", C_SUCCESS);




	// READ BENCHMARCK
	ConsoleOutput("Starting Read Benchmark", C_DEBUG);
	for (int i = 0; i < REQUEST_COUNT; i++)
	{
		QueryPerformanceFrequency(&frequency);
		// GET TIME BEFORE
		QueryPerformanceCounter(&t1);
		CallQuery("SELECT * FROM testtable where id=1");
		// GET TIME AFTER
		QueryPerformanceCounter(&t2);

		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		ConsoleOutputValue("Read", elapsedTime);

		// Clearing results
		MYSQL_RES *results;
		results = mysql_store_result(connection_mysql);
		mysql_free_result(results);

		// Save time 
		//benchmark_result[1][i] = elapsedTime;
	}
	ConsoleOutput("Success !", C_SUCCESS);

}
