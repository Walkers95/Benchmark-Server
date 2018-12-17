#include "jSONManager.h"


char* FormatJsonBenchmarkResults(struct database_current_results *results)
{
	char *string = NULL;
	cJSON *datas = NULL;
	cJSON *data = NULL;
	cJSON *write = NULL;
	cJSON *read = NULL;
	size_t index = 0;

	cJSON *json = cJSON_CreateObject();
	if (json == NULL)
	{
		goto end;
	}

	datas = cJSON_CreateArray();
	if (datas == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(json, "datas", datas);

	for (index = 0; index < results->request_number; ++index)
	{
		data = cJSON_CreateObject();
		if (data == NULL)
		{
			goto end;
		}
		cJSON_AddItemToArray(datas, data);

		write = cJSON_CreateNumber(results->results[0][index]);
		if (write == NULL)
		{
			goto end;
		}
		cJSON_AddItemToObject(data, "write", write);

		read = cJSON_CreateNumber(results->results[1][index]);
		if (read == NULL)
		{
			goto end;
		}
		cJSON_AddItemToObject(data, "read", read);
	}

	string = cJSON_Print(json);
	if (string == NULL)
	{
		fprintf(stderr, "Failed to print monitor.\n");
	}

end:
	cJSON_Delete(json);
	return string;
}

double * GetJsonBenchmarkResultsFromFile(char * file)
{
	return NULL;
}

void SaveJsonBenchmarkResults(char * file)
{
	printf("Saving file : %s \n", file);
}

double **GetJsonBenchmarkResults(char *json)
{
	const cJSON *results_json = cJSON_Parse(json);
	const cJSON *datas = NULL;
	const cJSON *data = NULL;

	double** returnArray = Malloc(2 * sizeof(double*));
	for (int i = 0; i < 2; i++)
	{
		returnArray[i] = Malloc(sizeof(double) * 1000);
	}

	if (results_json == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	int counter = 0;
	datas = cJSON_GetObjectItemCaseSensitive(results_json, "datas");
	cJSON_ArrayForEach(data, datas)
	{
		cJSON* read = cJSON_GetObjectItemCaseSensitive(data, "read");
		cJSON* write = cJSON_GetObjectItemCaseSensitive(data, "write");

		if (write && read)
		{
			returnArray[0][counter] = write->valuedouble;
			returnArray[1][counter] = read->valuedouble;

			counter++;
		}
		
	}

	cJSON_Delete(results_json);
	return returnArray;

end:
	free(returnArray);
	cJSON_Delete(datas);
	return NULL;
}

