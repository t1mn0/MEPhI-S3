#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs{

std::string GetTimeNow() {
  auto now = std::chrono::system_clock::now();
  auto tt = std::chrono::system_clock::to_time_t(now);
  std::tm* tm = std::localtime(&tt);

  char buffer[20];

  snprintf(buffer, sizeof(buffer), "%02d:%02d %02d.%02d.%02d",
    tm->tm_hour, tm->tm_min, tm->tm_mday, tm->tm_mon + 1, tm->tm_year % 100);

  return std::string(buffer);
}

std::string GetParentPath(const std::string& path) {
  size_t last_slash_position = path.rfind('/');

  if (last_slash_position == std::string::npos) {
    return path;
  }

  return path.substr(0, last_slash_position);
}

std::string CutFileName(const std::string& path) {
  size_t last_slash_position = path.rfind('/');

  if (last_slash_position == std::string::npos) {
    return path;
  }

  return path.substr(last_slash_position + 1);
}

}