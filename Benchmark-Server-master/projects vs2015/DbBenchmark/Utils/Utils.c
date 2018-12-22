#include "Utils.h"

// Memory management 
void * Malloc(size_t size)
{
	if (size <= 0)
		return NULL;

	void* adress = malloc(size);
	if (adress == NULL)
		MessageBoxA(0, "Memory allocation Error: Application could crash", "Error", 0);

	return adress;
}

void * Realloc(void * pointer, size_t size)
{
	if (size <= 0)
		return NULL;

	void *address = NULL;
	realloc(address, size);

	if (address == NULL)
		MessageBoxA(0, "Memory allocation Error: Application could crash", "Error", 0);
	
	return address;
}

char * GetTimeDate()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char* returnString = Malloc(255);
	sprintf(returnString, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	return returnString;
}

struct config_parameters* current_parameters = NULL;

void InitConfigParameters()
{
	current_parameters = Malloc(sizeof(struct config_parameters));
	current_parameters->checkbox_custom_script = 0;
	current_parameters->checkbox_ping = 0;
	current_parameters->checkbox_threads = 0;

	current_parameters->request_number = 0;
	current_parameters->selected_database = 0;
	
	for (int i = 0; i < 5; i++)
	{
		strcpy(current_parameters->input[i], "");
		current_parameters->text_len[i] = 0;
	}

	strcpy(current_parameters->box_buffer_read, "");
	strcpy(current_parameters->box_buffer_write, "");
	current_parameters->box_len_read = 0;
	current_parameters->box_len_write = 0;
}

void SetConfigParameters(struct config_parameters* params)
{
	current_parameters->checkbox_custom_script = params->checkbox_custom_script;
	current_parameters->checkbox_ping = params->checkbox_ping;
	current_parameters->checkbox_threads = params->checkbox_threads;

	current_parameters->request_number = params->request_number;
	current_parameters->selected_database = params->selected_database;

	for (int i = 0; i < 5; i++)
	{
		strcpy(current_parameters->input[i], params->input[i]);
		current_parameters->text_len[i] = params->text_len[i];
	}

	if (params->checkbox_custom_script)
	{
		strcpy(current_parameters->box_buffer_read, params->box_buffer_read);
		strcpy(current_parameters->box_buffer_write, params->box_buffer_write);
		current_parameters->box_len_read = params->box_len_read;
		current_parameters->box_len_write = params->box_len_write;
	}

}

struct config_parameters * GetConfigParameters()
{
	return current_parameters;
}


