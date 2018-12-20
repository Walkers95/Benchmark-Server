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

	int verif = InitMySql(db_param);
	free(db_param);

	return verif ? 1 : 0;
}

void DisconnectUserDatabase()
{
	mysql_close(connection_mysql);
	connection_mysql = NULL;
}

int LoginUser(struct database_login_params *db_login)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT ID FROM users WHERE name = '%s' AND password = '%s'", db_login->user, db_login->password);

	CallQuery(request);
	MYSQL_RES *results = mysql_store_result(connection_mysql);

	

	if (results == NULL)
		return 0;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		userID = atoi(row[0]);
		printf("User id : %d \n", userID);
	}
	ClearMySqlResults();
	DisconnectUserDatabase();

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

	for (int i = 0; i < GetUserBenchmarkCount(); i++)
	{
		userRecordsArray[i] = GetUserBenchmark(i);
	}


	return 1;
}


int UploadUserBenchmark(struct database_benchmark_params *benchmark, char* databaseType)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[99999];

	// insert dans la table benchmarks
	sprintf(request, 
	"INSERT INTO benchmarks(userID,Date,databaseType,hostname,port,databaseName,user,requestNumber,pingCompensation,customScript,multiThreads) VALUES(%d,'%s','%s','%s',%d,'%s','%s',%d,%d,%d,%d)"
		, userID,GetTimeDate(), databaseType, benchmark->hostname, benchmark->port, benchmark->database, benchmark->user, benchmark->request_number, benchmark->pingCompensation, benchmark->custom_script, benchmark->multi_threads);
	CallQuery(request);

	// Get ID of last inserted benchmarks
	sprintf(request, "SELECT id FROM benchmarks ORDER BY ID DESC LIMIT 1");
	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);
	if (results == NULL)
		return 0;

	int benchmarkID = 0;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		benchmarkID = atoi(row[0]);
	}
	ClearMySqlResults();

	// insert dans la table results
	sprintf(request, "INSERT INTO results(benchmark_id,score,results) VALUES(%d,%lf,'%s')"
		, benchmarkID,GetCurrentResults()->score, FormatJsonBenchmarkResults(GetCurrentResults()));
	CallQuery(request);

	DisconnectUserDatabase();

	return 1;
}

int DeleteUserBenchmark(int benchmarkID)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];

	// Delete data from benchmarks
	sprintf(request, "DELETE FROM benchmarks WHERE ID = %d", benchmarkID);
	CallQuery(request);

	// Delete data from results
	sprintf(request, "DELETE FROM results WHERE benchmark_id = %d", benchmarkID);
	CallQuery(request);

	DisconnectUserDatabase();

	return 1;
}




struct database_user_records* userRecords = NULL;

struct database_user_records *GetUserBenchmark(int benchmarkID)
{
	if (!ConnectUserDatabase())
		return 0;

	char request[255];
	sprintf(request, "SELECT * FROM benchmarks WHERE userID = %d ORDER BY ID DESC LIMIT %d,1",userID, benchmarkID);

	CallQuery(request);
	MYSQL_RES *results = NULL;
	results = mysql_store_result(connection_mysql);

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
	ClearMySqlResults();
	DisconnectUserDatabase();

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

	if (results == NULL)
		return 0;

	MYSQL_ROW* row = NULL;
	while (row = mysql_fetch_row(results))
	{
		benchmarkCount = atoi(row[0]);
		printf("User benchmark number : %d \n", benchmarkCount);
	}

	ClearMySqlResults();
	DisconnectUserDatabase();

	return benchmarkCount;
}

struct database_user_records ** GetUserBenchmarkData()
{
	return userRecordsArray;
}

int UpdateUserBenchmarkData()
{
	free(userRecordsArray);
	
	GetUserBenchmarkCountSql();
	userRecordsArray = Malloc(sizeof(struct database_user_records*) * GetUserBenchmarkCount());

	for (int i = 0; i < GetUserBenchmarkCount(); i++)
	{
		userRecordsArray[i] = Malloc(sizeof(struct database_user_records));
		userRecordsArray[i]->db_param = Malloc(sizeof(struct database_benchmark_params));
	}

	for (int i = 0; i < GetUserBenchmarkCount(); i++)
	{
		userRecordsArray[i] = GetUserBenchmark(i);
	}

	SetUpdateResultsData(0);
	return 1;
}

int GetUserBenchmarkCount()
{
	return benchmarkCount;
}

int GetUserID()
{
	return userID;
}

