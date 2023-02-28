#include "service.h"
#include "cli.h"

#include <Windows.h>

bool g_isService = GetConsoleWindow() == NULL;


/**
 * This program starts either as a service or a command line interface (CLI) application, depending on whether
 * a console window is present. If a console window is present, the program will start as a CLI application.
 * If no console window is present, the program will start as a service.
 * 
 * Args: 
 * 	argc (int): the number of command line arguments passed to the program
 * 	argv (char*[]): an array of strings containing the command line arguments passed to the program
 * 
 * Returns:
 * 	int: the return code of either the window_service or the cli function, depending on whether the program
 * 	started as a service or a CLI application */
int main(int argc, char* argv[])
{
	if (g_isService)
		// Started as a service
		return window_serive(argc, argv);
	
	else
		// Started as a CLI application
		return cli(argc, argv);
}
