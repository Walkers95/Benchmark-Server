#include "user_interface.h"


int userID = 0;
int benchmarkCount = 0;
struct database_user_records** userRecordsArray = NULL;

int ConnectUserDatabase()
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

	return 1;
}

void DisconnectUserDatabase()
{
	ClearMySqlResults();
	mysql_close(connection_mysql);
}

int LoginUser(struct database_login_params *db_login)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT ID FROM users WHERE name = '%s' AND password = '%s'", db_login->user, db_login->password);

	CallQuery(request);
	MYSQL_RES *results = mysql_store_result(connection_mysql);

	DisconnectUserDatabase();

	if (results == NULL)
		return 0;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		userID = atoi(row[0]);
		printf("User id : %d \n", userID);
	}

	if (!userID)
		return 0;

	// Get user benchmark results
	GetUserBenchmarkCountSql();
	userRecordsArray = Malloc(sizeof(struct database_user_records*) * GetUserBenchmarkCount());

	for ( int i = 0; i < GetUserBenchmarkCount(); i++)
	{
		userRecordsArray[i] = Malloc(sizeof(struct database_user_records));
		userRecordsArray[i]->db_param = Malloc(sizeof(struct database_benchmark_params));
	}

	for (int i = 1; i < GetUserBenchmarkCount() + 1; i++)
	{
		userRecordsArray[i-1] = GetUserBenchmark(i);
	}


	return 1;
}

int DeleteUserBenchmark(int benchmarkID)
{
	return 0;
}

double ** GetUserBenchmarkResults(int benchmarkID)
{
	return NULL;
}

struct database_user_records* userRecords = NULL;

struct database_user_records *GetUserBenchmark(int benchmarkID)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT * FROM benchmarks WHERE ID = '%d'", benchmarkID);

	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);

	DisconnectUserDatabase();

	if (results == NULL)
		return 0;

	userRecords = Malloc(sizeof(struct database_user_records));
	userRecords->db_param = Malloc(sizeof(struct database_benchmark_params));

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		userRecords->id = atoi(row[0]);
		userRecords->date = row[2];
		userRecords->databaseType = row[3];
		userRecords->db_param->hostname = row[4];
		userRecords->db_param->port = atoi(row[5]);
		userRecords->db_param->database = row[6];
		userRecords->db_param->user = row[7];
		userRecords->db_param->request_number = atoi(row[8]);
		userRecords->db_param->pingCompensation = atoi(row[9]);
		userRecords->db_param->custom_script = atoi(row[10]);
		userRecords->db_param->multi_threads = atoi(row[11]);	
	}

	return userRecords;
}

int GetUserBenchmarkCountSql()
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT COUNT(ID) FROM benchmarks WHERE userID = '%d'", userID);

	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);

	DisconnectUserDatabase();

	if (results == NULL)
		return 0;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		benchmarkCount = atoi(row[0]);
		printf("User benchmark number : %d \n", benchmarkCount);
	}

	return benchmarkCount;
}

struct database_user_records ** GetUserBenchmarkData()
{
	return userRecordsArray;
}

int GetUserBenchmarkCount()
{
	return benchmarkCount;
}

int GetUserID()
{
	return userID;
}

