#pragma once
#include <filesystem>
#include <string>


namespace my::system
{
	std::string getCurrentProgramFullPath();
	std::string getCurrentProgramPath();
	std::string getProgramPath();
	bool startProcess(const std::string& commandLine);
	std::string toString(const std::filesystem::path& path);
	std::string getEnvironmentVariable(const std::string& env);
	bool setEnvironmentVariable(const std::string& env, const std::string& value);
}