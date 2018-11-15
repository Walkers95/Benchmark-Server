#pragma once
#include "../Main/Includes.h"

struct console_data
{
	char* text;
	int length;
};

struct console_data *consData;

int InitConsole();
void ConsoleOutput(const char* text);
struct console_data *GetConsoleData();

void ClearConsole();