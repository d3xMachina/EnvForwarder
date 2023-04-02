#include <windows.h>
#include "maincommon.h"
#include "logger.h"


int wmain(int argc, wchar_t* argv[])
{
    if (!my::main::initLogs()) {
        return EXIT_FAILURE;
    }

    UINT consoleCP = GetConsoleOutputCP();
    if (!SetConsoleOutputCP(CP_UTF8)) {
        Log().Get(LOG_ERROR) << "Could not change the console codepage to UTF8";
    }

    Log().Get(LOG_INFO) << "Running in console mode";
    unsigned long exitCode = my::main::run(argc, argv);
    
    if (!SetConsoleOutputCP(consoleCP)) {
        Log().Get(LOG_ERROR) << "Could not revert the console codepage";
    }
    return exitCode;
}