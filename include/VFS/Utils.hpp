#pragma once

#include <string>

namespace tmn_vfs{

std::string GetTimeNow();

std::string GetParentPath(const std::string& path);

};