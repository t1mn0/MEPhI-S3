#pragma once

#include <string>

namespace tmn {
namespace vfs {

std::string get_time_now();

bool full_match(const std::string& file_name, const std::string& pattern);

bool wildcard_match(const std::string& file_name, const std::string& pattern);

bool file_name_match(const std::string& file_name, const std::string& pattern);

bool is_good_username(const std::string& username);

bool is_good_fullname(const std::string& fullname);

bool is_good_filename(const std::string& filename);

bool is_good_password(const std::string& password);

bool is_regular_writable_binfile(const std::string& path);

}
}