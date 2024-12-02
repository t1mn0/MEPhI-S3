#pragma once

#include <string>

namespace tmn_vfs {

std::string GetTimeNow();

bool FullMatch(const std::string& file_name, const std::string& pattern);

bool WildcardMatch(const std::string& file_name, const std::string& pattern);

bool FileNameMatch(const std::string& file_name, const std::string& pattern);

bool IsGoodUserName(const std::string& username);

bool IsGoodUserFullName(const std::string& fullname);

bool IsGoodFileName(const std::string& filename);

bool IsGoodPassword(const std::string& password);

}