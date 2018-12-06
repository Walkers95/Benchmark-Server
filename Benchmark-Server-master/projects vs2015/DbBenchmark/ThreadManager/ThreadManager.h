#pragma once
#include "../Main/Includes.h"

#include "../Console/Console.h"
#include "../Interfaces/Interfaces.h"

#include "../Library/SDL2-2.0.8/include/SDL_thread.h"




int MySqlThread(void* data);
int OracleThread(void* data);

void StartBenchmarkThread(struct database_benchmark_params *db_param, char* dbType);
void StartQueryThread();