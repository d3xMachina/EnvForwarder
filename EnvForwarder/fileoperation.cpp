#include "fileoperation.h"

namespace fs = std::filesystem;

namespace my::fileoperations
{
bool createDirectories(const fs::path& path)
{
    bool ok = true;
    if (!fs::is_directory(path)) {
        ok = fs::create_directories(path);
    }
    return ok;
}
}
