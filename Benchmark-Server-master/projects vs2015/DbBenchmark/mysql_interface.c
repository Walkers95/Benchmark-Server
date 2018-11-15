#include "mysql_interface.h"

double **benchmark_result;

void InitMySql(char * hostname, int port, char * dbName, char * user, char * password)
{
	connection_mysql = mysql_init(NULL);

	if (connection_mysql == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(connection_mysql));
		exit(1);
	}

	if (hostname == "localhost")
	{
		if (mysql_real_connect(connection_mysql, hostname, user, password, NULL, 0, NULL, 0) == NULL)
		{
			fprintf(stderr, "%s\n", mysql_error(connection_mysql));
			mysql_close(connection_mysql);
			exit(1);
		}
	} 
	else
	{
		if (mysql_real_connect(connection_mysql, hostname, user, password, dbName, port, NULL, 0) == NULL)
		{
			fprintf(stderr, "%s\n", mysql_error(connection_mysql));
			mysql_close(connection_mysql);
			exit(1);
		}
	}
	

}

void FinishWithError()
{
	fprintf(stderr, "%s\n", mysql_error(connection_mysql));
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
	printf("> Making sure testdb does not exist !");
	CallQuery("DROP DATABASE testdb");
	printf("> Success !");
	
	// on crée notre database de test
	printf("> Creating test database \n");
	CallQuery("CREATE DATABASE testdb");
	printf("> Success ! \n");

	// On selectionne la db crée
	printf("> Selection de la base \n");
	CallQuery("USE testdb");
	printf("> Success ! \n");

	// Create table 
	printf("> Creating test table \n");
	CallQuery("CREATE TABLE testtable(id INTEGER AUTO_INCREMENT PRIMARY KEY,int_test INTEGER,text_test TEXT)");
	printf("> Success ! \n");

	

	// WRITE BENCHMARK
	printf("> Starting Write Benchmark \n");
	
	for (int i = 0; i < REQUEST_COUNT; i++)
	{
		QueryPerformanceFrequency(&frequency);
		// GET TIME BEFORE
		QueryPerformanceCounter(&t1);
		CallQuery("INSERT INTO testtable(int_test, text_test) VALUES(1,'bonjour')");
		// GET TIME AFTER
		QueryPerformanceCounter(&t2);
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		printf("(Write) : %lf ms \n", elapsedTime);

		// Save time 
		//benchmark_result[0][i] = elapsedTime;
	}
	printf("> Success ! \n");




	// READ BENCHMARCK
	printf("> Starting Read Benchmark \n");
	for (int i = 0; i < REQUEST_COUNT; i++)
	{
		QueryPerformanceFrequency(&frequency);
		// GET TIME BEFORE
		QueryPerformanceCounter(&t1);
		CallQuery("SELECT * FROM testtable where id=1");
		// GET TIME AFTER
		QueryPerformanceCounter(&t2);

		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		printf("(Read) : %lf ms \n", elapsedTime);

		// Clearing results
		MYSQL_RES *results;
		results = mysql_store_result(connection_mysql);
		mysql_free_result(results);

		// Save time 
		//benchmark_result[1][i] = elapsedTime;
	}
	printf("> Success ! \n");

}
