#include "service.h"
#include "cli.h"

#include <Windows.h>


int main(int argc, char* argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {service_name, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    if (!StartServiceCtrlDispatcher(ServiceTable))
    {
        if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
        {
            logger->info("CLI start-up");
            return cli(argc, argv);
        }

        else
        {
            logger->error("Faild to start program");
            return -1;
        }

        return 0;
    }
}
