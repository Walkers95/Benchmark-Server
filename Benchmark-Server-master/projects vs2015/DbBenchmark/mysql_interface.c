#include "mysql_interface.h"

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

void DoBenchmarkMySql()
{
	printf(" Starting DoBenchmark \n");
	if (mysql_query(connection_mysql, "CREATE DATABASE testdb"))
	{
		fprintf(stderr, "%s\n", mysql_error(connection_mysql));
		mysql_close(connection_mysql);
		exit(1);
	}
}
