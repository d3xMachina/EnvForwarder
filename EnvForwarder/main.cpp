#include <Windows.h>
#include "maincommon.h"
#include "logger.h"


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    if (!my::main::initLogs()) {
        return EXIT_FAILURE;
    }

    Log().Get(LOG_INFO) << "Running in GUI mode";

    unsigned long exitCode = my::main::run(__argc, __wargv);
    return exitCode;
}
