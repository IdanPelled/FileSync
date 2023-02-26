#include "service.h"
#include "cli.h"

#include <Windows.h>

bool g_isService = GetConsoleWindow() == NULL;


int main(int argc, char* argv[])
{
	if (g_isService)
		// Started as a service
		return window_serive(argc, argv);
	
	else
		// Started as a CLI application
		return cli(argc, argv);
}
