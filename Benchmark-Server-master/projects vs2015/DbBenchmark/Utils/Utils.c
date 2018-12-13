#include "Utils.h"

// Memory manegement 
void * Malloc(size_t size)
{
	void* adress = malloc(size);
	if (adress == NULL)
		exit(0);

	return adress;
}

void * Realloc(void * pointer, size_t size)
{
	void *address = NULL;
	realloc(address, size);

	if (address == NULL)
		exit(0);

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


