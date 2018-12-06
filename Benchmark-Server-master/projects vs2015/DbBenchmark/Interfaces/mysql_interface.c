#include "mysql_interface.h"

int InitMySql(struct database_benchmark_params *db_param)
{
	connection_mysql = mysql_init(NULL);

	if (connection_mysql == NULL)
	{
		ConsoleOutput(mysql_error(connection_mysql), C_ERROR);
		return 0;
	}

	if (db_param->hostname == "localhost")
	{
		if (mysql_real_connect(connection_mysql, db_param->hostname, db_param->user, db_param->password, db_param->database, 0, NULL, 0) == NULL)
		{
			ConsoleOutput(mysql_error(connection_mysql), C_ERROR);
			mysql_close(connection_mysql);
			return 0;
		}
	} 
	else
	{
		if (mysql_real_connect(connection_mysql, db_param->hostname, db_param->user, db_param->password, db_param->database, db_param->port, NULL, 0) == NULL)
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
	printf("%s \n", mysql_error(connection_mysql));
	//mysql_close(connection_mysql);
	fgets(NULL, 0, stdin);
}

void ClearMySqlResults()
{
	// Clearing results
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);
	mysql_free_result(results);
}

int CallQuery(const char * query)
{
	if (mysql_query(connection_mysql, query)) {
		FinishWithError();
		return 0;
	}
	return 1;
}



int DoBenchmarkMySql(struct database_benchmark_params *db_param)
{
	float score = 0;
	double **benchmark_result = NULL;

	// Init benchmark result
	benchmark_result = Malloc(sizeof(double*) * 2);
	for (int i = 0; i < 2; i++)
	{
		benchmark_result[i] = Malloc(sizeof(double) * db_param->request_number);
	}

	// On DROP la table testtable au cas ou elle existe déja
	ConsoleOutput("Making sure testtable does not exist !", C_DEBUG);
	CallQuery("DROP TABLE testtable");
	ConsoleOutput("Success !", C_SUCCESS);

	// Create table 
	ConsoleOutput("Creating test table", C_DEBUG);
	if(!CallQuery("CREATE TABLE testtable(id INTEGER AUTO_INCREMENT PRIMARY KEY,int_test INTEGER,text_test TEXT)"))
		return 0;
	ConsoleOutput("Success !", C_SUCCESS);



	// WRITE BENCHMARK
	ConsoleOutput("Starting Write Benchmark", C_DEBUG);
	for (int i = 0; i < db_param->request_number; i++)
	{
		QueryPerformanceFrequency(&frequency);

		if (db_param->custom_script)
		{
			QueryPerformanceCounter(&t1); // GET TIME BEFORE
			CallQuery(db_param->scrit_write);
			QueryPerformanceCounter(&t2); // GET TIME AFTER
		}
		else 
		{
			QueryPerformanceCounter(&t1); // GET TIME BEFORE
			CallQuery("INSERT INTO testtable(int_test, text_test) VALUES(1,'bonjour')");
			QueryPerformanceCounter(&t2); // GET TIME AFTER
		}
	
		elapsedTime = ((t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);
		ConsoleOutputValue("Write", elapsedTime);
		score += 1 / (elapsedTime / 1000);
	
		// Save time 
		benchmark_result[0][i] = elapsedTime;
	}
	ConsoleOutput("Success !", C_SUCCESS);




	// READ BENCHMARCK
	ConsoleOutput("Starting Read Benchmark", C_DEBUG);
	for (int i = 0; i < db_param->request_number; i++)
	{
		QueryPerformanceFrequency(&frequency);
		
		if (db_param->custom_script)
		{
			QueryPerformanceCounter(&t1); // GET TIME BEFORE
			CallQuery(db_param->script_read);
			QueryPerformanceCounter(&t2); // GET TIME AFTER
		}
		else
		{
			QueryPerformanceCounter(&t1); // GET TIME BEFORE
			CallQuery("SELECT * FROM testtable where id=1");
			QueryPerformanceCounter(&t2); // GET TIME AFTER
		}
		

		elapsedTime = ((t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);
		ConsoleOutputValue("Read", elapsedTime);
		score += 1 / (elapsedTime / 1000);

		ClearMySqlResults();

		// Save time 
		benchmark_result[1][i] = elapsedTime;
	}

	if (db_param->pingCompensation == 1)
	{
		QueryPerformanceFrequency(&frequency);
		// GET TIME BEFORE
		QueryPerformanceCounter(&t1);
		CallQuery("SELECT 1");
		// GET TIME AFTER
		QueryPerformanceCounter(&t2);

		ClearMySqlResults();

		pingLatency = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		printf("ping latency : %lf \n", pingLatency);

		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < db_param->request_number;i++)
			{
				benchmark_result[j][i] -= pingLatency;
			}
		}

	}


	score /= db_param->request_number;
	SetRequestNumber(db_param->request_number);
	SetScore(score);
	SetResults(benchmark_result);

	printf("Score : %f %n", score);
	ConsoleOutput("Success !", C_SUCCESS);


	mysql_close(connection_mysql);
	return 1;
}

int Login(struct database_login_params *db_login)
{
	struct database_benchmark_params* db_param = Malloc(sizeof(struct database_benchmark_params));
	db_param->hostname = "localhost";
	db_param->user = "root";
	db_param->password = "";
	db_param->database = "DBBenchmark";
	
	if (!InitMySql(db_param))
	{
		return 0;
	}

	char request[255];
	sprintf(request, "SELECT ID FROM users WHERE name = '%s' AND password = '%s'", db_login->user, db_login->password);

	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);

	ClearMySqlResults();
	mysql_close(connection_mysql);

	if (results == NULL)
		return 0;

	int user_id = 0;
	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		user_id = atoi(row[0]);
		printf("User id : %d", user_id);
	}

	if (!user_id)
		return 0;

	return 1;
}
