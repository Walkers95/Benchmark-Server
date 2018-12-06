#pragma once

#include "Interfaces.h"
// Oracle include
#include "../Library\Oracle\include\ocilib.h"

OCI_Connection* connection_oracle;
OCI_Statement* statement;
OCI_Resultset* result;

void InitOracle(struct database_benchmark_params *db_param);

void DoBenchmarkOracle();