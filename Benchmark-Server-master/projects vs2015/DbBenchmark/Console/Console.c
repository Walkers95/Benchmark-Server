#include "Console.h"


int InitConsole()
{
	printf("> Console initialisation \n");

	consData.text = malloc(11);

	if (!consData.text)
		return 0;

	strcpy(consData.text, "Hello world");
	consData.length = 11;


	consData.type = 1;

	return 1;
}

void ConsoleOutput(const char * text, enum output_type type)
{
	char *line_intro = NULL;

	switch (type)
	{
	case C_ERROR:
		line_intro = malloc(11);
		strcpy(line_intro,"[ERROR] > ");
		break;
	case C_DEBUG:
		line_intro = malloc(11);
		strcpy(line_intro, "[DEBUG] > ");
		break;
	case C_SUCCESS:
		line_intro = malloc(13);
		strcpy(line_intro, "[SUCCESS] > ");
		break;
	}
	
	size_t textSize = strlen(text);
	size_t line_intro_size = strlen(line_intro);

	size_t newSize = consData.length + textSize + line_intro_size + 1; // Ajout 1 pour \n

	const char* oldText = malloc(consData.length); 
	strcpy(oldText,consData.text);

	free(consData.text);
	consData.text = malloc(newSize);
	strcpy(consData.text, line_intro);
	strcat(consData.text, text);
	strcat(consData.text, "\n");
	strcat(consData.text, oldText);

	consData.length = newSize;;

	consData.type = type;
}

struct console_data GetConsoleData()
{
	return consData;
}

void ClearConsole()
{
	realloc(consData.text, 2);
	consData.length = 2;
	strcpy(consData.text, "");
}
