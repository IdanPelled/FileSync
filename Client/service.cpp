#include "service.h"


extern std::shared_ptr<spdlog::logger> logger;


#define SERVICE_NAME L"MyService"
LPWSTR service_name = const_cast<LPWSTR>(SERVICE_NAME);


SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
VOID WINAPI ServiceCtrlHandler(DWORD);
DWORD WINAPI ServiceWorkerThread(LPVOID);


int window_serive(int argc, char* argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {service_name, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    StartServiceCtrlDispatcher(ServiceTable);

    return 0;
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
    logger->info("in ServiceMain");
    DWORD Status = E_FAIL;

    ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

    if (ServiceStatusHandle == NULL)
    {
        return;
    }

    ZeroMemory(&ServiceStatus, sizeof(ServiceStatus));
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwServiceSpecificExitCode = 0;

    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

    if (hThread == NULL)
    {
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = GetLastError();
        SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
        return;
    }

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    return;
}

VOID WINAPI ServiceCtrlHandler(DWORD ControlCode)
{
    logger->info("in ServiceCtrlHandler");
    switch (ControlCode)
    {
    case SERVICE_CONTROL_STOP:
        ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
        // PostThreadMessage(dwThreadId, WM_QUIT, 0, 0);
        break;
    }
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
    logger->info("in ServiceWorkerThread");
    Sleep(3000);

    return ERROR_SUCCESS;
}
