#include "main.h"

int main(int argc, char* argv[])
{

	InitialiseRender();

	if (!InitConsole())
		return 1;
	
	Render();

	return 0;
}
