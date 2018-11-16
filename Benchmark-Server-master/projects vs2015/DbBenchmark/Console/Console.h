#pragma once
#include "../Main/Includes.h"

const enum output_types
{
	C_ERROR = 0,
	C_DEBUG,
	C_SUCCESS,
}output_type;

struct console_data
{
	char* text;
	int length;
	enum output_type type;
};

struct console_data consData;

int InitConsole();

// Add args to string
// https://stackoverflow.com/questions/7031116/how-to-create-function-like-printf-variable-argument
void ConsoleOutput(const char* text, enum output_type type);
struct console_data GetConsoleData();

void ClearConsole();