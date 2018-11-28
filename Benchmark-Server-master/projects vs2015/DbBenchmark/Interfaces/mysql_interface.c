#include "mysql_interface.h"

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

	// Init benchmark result
	benchmark_result = malloc(sizeof(double*) * 2);
	for (int i = 0; i < 2; i++)
	{
		benchmark_result[i] = malloc(sizeof(double) * db_param->request_number);
	}

	param_request_number = db_param->request_number;
	param_ping = db_param->pingCompensation;
	param_custom_script = db_param->custom_script;

	if (param_custom_script)
	{
		param_script_read = db_param->script_read;
		param_script_write = db_param->scrit_write;
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

void ClearResults()
{
	// Clearing results
	MYSQL_RES *results;
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

double GetMinValueOfResults(double** results) {
	double min = 1.00f;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < param_request_number; j++)
		{
			if (results[i][j] != 0 && results[i][j] < min)
			{
				min = results[i][j];
			}
		}
	}

	return min;
}

double GetMaxValueOfResults(double** results) {
	double max = 0.0f;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < param_request_number; j++)
		{
			if (results[i][j] > max)
			{
				max = results[i][j];
			}
		}
	}

	return max;
}

double ** GetResults()
{
	return benchmark_result;
}


int DoBenchmarkMySql()
{

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
	for (int i = 0; i < param_request_number; i++)
	{
		QueryPerformanceFrequency(&frequency);

		if (param_custom_script)
		{
			QueryPerformanceCounter(&t1); // GET TIME BEFORE
			CallQuery(param_script_write);
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
	
		// Save time 
		benchmark_result[0][i] = elapsedTime;
	}
	ConsoleOutput("Success !", C_SUCCESS);




	// READ BENCHMARCK
	ConsoleOutput("Starting Read Benchmark", C_DEBUG);
	for (int i = 0; i < param_request_number; i++)
	{
		QueryPerformanceFrequency(&frequency);
		
		if (param_custom_script)
		{
			QueryPerformanceCounter(&t1); // GET TIME BEFORE
			CallQuery(param_script_read);
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
	
		ClearResults();

		// Save time 
		benchmark_result[1][i] = elapsedTime;
	}

	if (param_ping == 1)
	{
		QueryPerformanceFrequency(&frequency);
		// GET TIME BEFORE
		QueryPerformanceCounter(&t1);
		CallQuery("SELECT 1");
		// GET TIME AFTER
		QueryPerformanceCounter(&t2);

		ClearResults();

		pingLatency = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		printf("ping latency : %lf \n", pingLatency);
	}


	ConsoleOutput("Success !", C_SUCCESS);



	return 1;
}
