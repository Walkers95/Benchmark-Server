#pragma once
#include "../Main/Includes.h"


// Memory manegement 
void* Malloc(size_t size);
void* Realloc(void* pointer, size_t size);

char* GetTimeDate();

struct config_parameters
{
	// Pour les champs de selections de text : 
	char input[5][64];
	int text_len[5];
	char box_buffer_read[512];
    int box_len_read;
	char box_buffer_write[512];
	int box_len_write;
	int checkbox_threads;
	int checkbox_ping;
	int checkbox_custom_script;
	int selected_database;
	int request_number;
};

void InitConfigParameters();
void SetConfigParameters(struct config_parameters* params);
struct config_parameters* GetConfigParameters();
