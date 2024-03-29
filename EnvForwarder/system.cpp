#include "system.h"
#include "logger.h"
#include "stringmanip.h"
#include <fstream>
#include <Windows.h>

namespace fs = std::filesystem;
namespace sm = my::stringmanip;

namespace my::system
{
std::string getCurrentProgramFullPath()
{
    std::string strPath;
    HMODULE hModule = GetModuleHandleW(NULL);
    if (hModule != NULL) {
        std::wstring buffer;
        buffer.resize(MAX_PATH);

        GetModuleFileNameW(hModule, &buffer[0], (DWORD)buffer.size());
        sm::removeTrailingNull(buffer);
        strPath = sm::toString(buffer);
    }
    return strPath;
}

std::string getCurrentProgramPath()
{
    static std::string strPath;

    if (strPath.empty()) {
        fs::path path = fs::u8path(getCurrentProgramFullPath());
        path.remove_filename();
        strPath = toString(path);
    }
    return strPath;
}

std::string getProgramPath()
{
    std::string strPath;
    std::ifstream fProgram(getCurrentProgramPath() + "EnvForwarder\\path.txt", std::ios::in);
    if (fProgram.is_open()) {
        std::getline(fProgram, strPath);
        fs::path path = fs::u8path(strPath);
        if (path.has_filename() && !path.has_parent_path()) {
            strPath = getCurrentProgramPath() + strPath;
        }
    }
    return strPath;
}

bool startProcess(const std::string& commandLine, unsigned long& exitCode)
{
    std::wstring WCommandLine = sm::toWString(commandLine);
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    if (!CreateProcessW(
        NULL,
        &WCommandLine[0],
        NULL,
        NULL,
        FALSE,
        CREATE_UNICODE_ENVIRONMENT,
        NULL,
        NULL,
        &si,
        &pi)
        )
    {
        return false;
    }

    // Close child process when parent dies
    HANDLE hJob = CreateJobObject(NULL, NULL);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobExtendedLimitInfo = { 0 };
    jobExtendedLimitInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jobExtendedLimitInfo, sizeof(jobExtendedLimitInfo)))
    {
        Log().Get(LOG_WARNING) << "Could not make a process job";
    }
    else if (!AssignProcessToJobObject(hJob, pi.hProcess))
    {
        Log().Get(LOG_WARNING) << "Could not set the process job";
    }

    // Wait for the process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get the exit code of the process
    bool exitCodeOk = GetExitCodeProcess(pi.hProcess, &exitCode);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hJob);

    if (!exitCodeOk) {
        exitCode = EXIT_FAILURE;
        return false;
    }
    return true;
}

std::string toString(const fs::path& path)
{
    std::u8string u8Path = path.u8string();
    return sm::toString(u8Path);
}

std::string getEnvironmentVariable(const std::string& env)
{
    std::wstring wEnv = sm::toWString(env);
    std::string value;
    int size = GetEnvironmentVariableW(&wEnv[0], NULL, 0);
    if (size > 0) {
        std::wstring buffer;
        buffer.resize(size);
        GetEnvironmentVariableW(&wEnv[0], &buffer[0], (DWORD)buffer.size());
        sm::removeTrailingNull(buffer);
        value = sm::toString(buffer);
    }
    return value;
}

bool setEnvironmentVariable(const std::string& env, const std::string& value)
{
    std::wstring wEnv = sm::toWString(env);
    std::wstring wValue = sm::toWString(value);
    bool ok = SetEnvironmentVariableW(&wEnv[0], &wValue[0]);
    return ok;
}
}