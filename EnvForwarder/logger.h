#pragma once
#include <sstream>

enum LogLevel {
    LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR
};

class Log
{
public:
    Log();
    virtual ~Log();
    std::ostringstream& Get(LogLevel level = LOG_INFO);

public:
    static void setDestination(const std::string& dest);
    static void setReportingLevel(LogLevel level);
    static void clear();

protected:
    std::ostringstream os;

private:
    static std::string dest;
    static LogLevel reportingLevel;
    LogLevel messageLevel;

    static std::string currentDateTime();
    static std::string toString(LogLevel level);
    Log(const Log&) = delete;
    Log& operator =(const Log&) = delete;
};
