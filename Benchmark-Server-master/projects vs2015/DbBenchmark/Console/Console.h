#pragma once
#include "../Main/Includes.h"
#include "../Utils/Utils.h"

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

struct console_data *consData;

int InitConsole();

void ConsoleOutputValue(const char* text, double value);
void ConsoleOutput(const char* text,enum output_type type);

struct console_data *GetConsoleData();

void ClearConsole();