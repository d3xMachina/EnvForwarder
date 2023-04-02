#include "fileoperation.h"
#include "logger.h"
#include "rules.h"
#include "stringmanip.h"
#include "system.h"

namespace fs = std::filesystem;
namespace fo = my::fileoperations;
namespace sm = my::stringmanip;
namespace sys = my::system;

namespace my::main
{
bool createDirectories()
{
    fs::path path = fs::u8path(sys::getCurrentProgramPath() + "EnvForwarder"); // no trailing slash or error on creation of dir even if it works
    bool ok = fo::createDirectories(path);
    if (!ok) {
        Log().Get(LOG_ERROR) << "Failed to create directory " << sys::toString(path);
    }
    return ok;
}
bool initLogs()
{
    Log::setDestination(sys::getCurrentProgramPath() + "EnvForwarder\\logs.log");
#ifdef _DEBUG
    Log::setReportingLevel(LOG_DEBUG);
#else
    Log::setReportingLevel(LOG_INFO);
#endif
    Log::clear();

    if (!createDirectories()) {
        return false;
    }
    return true;
}

unsigned long run(int argc, wchar_t* argv[])
{
    std::string program = sys::getProgramPath();
    fs::path programPath = fs::u8path(program);
    if (!fs::is_regular_file(programPath)) {
        Log().Get(LOG_ERROR) << "Program missing or not specified! (create a file path.txt with the path of the absolute path of the program)";
        return EXIT_FAILURE;
    }

    // Get arguments
    std::string args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = sm::toString(std::wstring(argv[i]));
        arg = "\"" + arg + "\"";
        args += " " + arg;
    }
    std::string commandLine = "\"" + program + "\"" + args;

    // Apply custom rules to environment variables
    my::EnvironmentRules rules;
    if (!rules.open(sys::getCurrentProgramPath() + "EnvForwarder\\rules.txt")) {
        return EXIT_FAILURE;
    }
    if (!rules.apply()) {
        Log().Get(LOG_ERROR) << "Could not apply rules";
        return EXIT_FAILURE;
    }

    unsigned long exitCode = EXIT_SUCCESS;
    bool ok = sys::startProcess(commandLine, exitCode);
    if (!ok) {
        Log().Get(LOG_ERROR) << "Could not launch the application " << program << " with arguments " << args;
        return EXIT_FAILURE;
    }
    Log().Get(LOG_INFO) << "Exit code=" << exitCode;
    return exitCode;
}
}