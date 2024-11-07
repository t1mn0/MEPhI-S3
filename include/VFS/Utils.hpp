#pragma once

namespace tmn_vfs{

std::string GetTimeNow();

std::string GetParentPath(const std::string& path);

std::string CutFileName(const std::string& path);

}