#include <ctime>
#include <chrono>
#include <iomanip>

#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs{

std::string GetTimeNow() {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&tt);

    char buffer[20];

    snprintf(buffer, sizeof(buffer), "%02d-%02d-%02d-%02d-%02d-%d",
             tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

    return std::string(buffer);
}

}