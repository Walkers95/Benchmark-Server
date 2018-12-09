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


