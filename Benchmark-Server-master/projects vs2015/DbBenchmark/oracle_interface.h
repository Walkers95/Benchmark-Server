#pragma once
// Oracle include
#include "Library\Oracle\include\ocilib.h"

OCI_Connection* connection_oracle;
OCI_Statement* statement;
OCI_Resultset* result;

void InitOracle(char* hostname, int port, char* dbName, char* user, char* password);

void DoBenchmarkOracle();