#pragma once
#include "logger.h"

#include <windows.h>
#include <stdio.h>

/**
 * Runs the program as a Windows service.
 *
 * This function sets up the service table entry and calls the Windows API function
 * StartServiceCtrlDispatcher to start the service controller dispatcher thread, which
 * waits for service events such as start, stop, and pause requests from the operating system.
 *
 * @param argc The number of command line arguments passed to the program.
 * @param argv An array of pointers to the command line arguments passed to the program.
 *
 * @return An integer indicating the exit code of the program.
 */
int window_serive(int argc, char* argv[]);