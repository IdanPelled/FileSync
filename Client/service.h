#pragma once
#include "logger.h"
#include "sync.h"

#include <windows.h>
#include <tchar.h>


VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
VOID WINAPI ServiceCtrlHandler(DWORD);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

#define SERVICE_NAME  L"FileSync"
extern LPWSTR service_name;