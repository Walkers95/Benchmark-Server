#include "Console.h"


int InitConsole()
{
	printf("> Console initialisation \n");

	consData.text = malloc(1);

	if (!consData.text)
		return 0;

	strcpy(consData.text, "");
	consData.length = 1;

	consData.type = 1;

	return 1;
}

void ConsoleOutputValue(const char * text, double value)
{
	char *double_buffer = malloc(255);
	sprintf(double_buffer, " : %lf ms", value);

	char* buffer = malloc(strlen(text) + strlen(double_buffer));;
	strcpy(buffer, text);
	strcat(buffer, double_buffer);
	free(double_buffer);

	ConsoleOutput(buffer, C_DEBUG);
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

	consData.length = newSize;
	consData.type = type;

	free(oldText);
	free(line_intro);
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
