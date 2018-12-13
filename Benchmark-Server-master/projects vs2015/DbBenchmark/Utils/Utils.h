#pragma once
#include "../Main/Includes.h"

// Memory manegement 
void* Malloc(size_t size);
void* Realloc(void* pointer, size_t size);

char* GetTimeDate();