#include "logger.h"
#include <ctime>
#include <fstream>


LogLevel Log::reportingLevel = LOG_INFO;
std::string Log::dest = "logs.log";

Log::Log() : messageLevel(LOG_INFO) {}

Log::~Log()
{
    if (messageLevel >= reportingLevel)
    {
        os << std::endl;
        std::ofstream fLog(dest, std::ios::out | std::ios::app);
        if (fLog.is_open()) {
            fLog << os.str();
            fLog.flush();
        }
    }
}

void Log::setDestination(const std::string& dest)
{
    Log::dest = dest;
}

void Log::setReportingLevel(LogLevel level)
{
    reportingLevel = level;
}

void Log::clear()
{
    std::ofstream fLog(dest, std::ios::out | std::ios::trunc);
}

std::string Log::currentDateTime()
{
    /* Modern way to get the current date but triple the file size
    const auto now = std::chrono::system_clock::now();
    std::format("{:%d-%m-%Y %H:%M:%OS}", now);
    */
    time_t now = time(0);
    char buffer[80];
    struct tm tstruct;
    localtime_s(&tstruct, &now);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %X", &tstruct);

    return buffer;
}

std::ostringstream& Log::Get(LogLevel level)
{
    os << currentDateTime();
    os << " " << toString(level) << ": ";
    messageLevel = level;
    return os;
}

std::string Log::toString(LogLevel level)
{
    std::string strLevel = "UNKNOWN";
    switch (level)
    {
    case LOG_ERROR:     strLevel = "ERROR"; break;
    case LOG_WARNING:   strLevel = "WARNING"; break;
    case LOG_INFO:      strLevel = "INFO"; break;
    case LOG_DEBUG:     strLevel = "DEBUG"; break;
    }
    return strLevel;
}
