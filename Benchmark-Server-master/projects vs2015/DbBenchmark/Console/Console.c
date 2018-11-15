#include "Console.h"


int InitConsole()
{
	printf("> Console initialisation \n");
	consData = malloc(sizeof *consData);
	if (!consData)
		return 0;

	consData->text = malloc(12);

	if (!consData->text)
		return 0;

	strcpy(consData->text, "Hello world");
	consData->length = 12;

	return 1;
}

void ConsoleOutput(const char * text)
{
	printf("> Adding text to console : %s \n", text);
	int textSize = strlen(text);
	printf("textSize : %d \n", textSize);
	size_t newSize = consData->length + textSize + 500; // Ajout 1 pour \n
	printf("newSize : %d \n", newSize);

	realloc(consData->text, newSize);
	strcat(consData->text, text);
	printf("consData->text : %s \n", consData->text);
	strcat(consData->text, "\n");
	printf("consData->text : %s \n", consData->text);

	consData->length = newSize;
	printf("consData->length : %d \n", consData->length);
}

struct console_data *GetConsoleData()
{
	return consData;
}

void ClearConsole()
{
	realloc(consData->text, 1);
	consData->length = 1;
	strcpy(consData->text, "");
}
