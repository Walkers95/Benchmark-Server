#include "oracle_interface.h"

void InitOracle(char * hostname, int port, char * dbName, char * user, char * password)
{

	OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);

	connection_oracle = OCI_ConnectionCreate("(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL =TCP)(HOST = localhost)(PORT = 80)))(CONNECT_DATA = (SERVICE_NAME =MY_SERVICE_NAME)))", "usr", "pwd", OCI_SESSION_DEFAULT);
	statement = OCI_StatementCreate(connection_oracle);

}

void DoBenchmarkOracle()
{
	printf(" Starting DoBenchmark for Oracle \n");
}
