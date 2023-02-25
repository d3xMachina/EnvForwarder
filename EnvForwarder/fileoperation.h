#pragma once
#include <filesystem>
#include <string>

namespace my::fileoperations
{
	bool createDirectories(const std::filesystem::path& path);
}