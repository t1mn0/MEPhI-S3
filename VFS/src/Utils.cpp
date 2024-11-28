#include <ctime>
#include <chrono>
#include <iomanip>
#include <regex>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>

#include "../include/Utils.hpp"
#include "../include/vfs_constants.hpp"

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

bool FullMatch(const std::string& file_name, const std::string& pattern) {
  return file_name == pattern;
}

bool WildcardMatch(const std::string& str, const std::string& pattern) {
  if (pattern == "*") return !str.empty();

  size_t star_pos = pattern.find('*');

  if ((star_pos != 0) && (star_pos != pattern.length() - 1)) {
    return false;
  }

  if (star_pos == 0) {
    return (str.length() >= pattern.length() - 1) && (str.substr(str.length() - (pattern.length() - 1)) == pattern.substr(1));
  } 
  else {
    return (str.length() >= pattern.length() - 1) && (str.substr(0, pattern.length() - 1) == pattern.substr(0, pattern.length() -1));
  }
}

bool FileNameMatch(const std::string& file_name, const std::string& pattern) {
  if (pattern.find('*') == std::string::npos) {
    return FullMatch(file_name, pattern);
  } 
  return WildcardMatch(file_name, pattern);
}

bool IsGoodUserName(const std::string& username){
  if (username.empty()) {
    return false;
  }

  if (username.length() < 2 || username.length() > 16) {
    return false;
  }

  for (char c : username) {
    if (!isalnum(c) && c != '_') {
      return false;
    }
  }

  if (!isalpha(username[0])) {
    return false;
  }

  if(username == SUPER_USERNAME){
    return false;
  }

  return true;
}

bool IsGoodUserFullName(const std::string& fullname){
  if (fullname.empty()) {
    return false;
  }

  if (fullname.length() > 32) {
    return false;
  }

  for (char c : fullname) {
    if (!isalpha(c) && !isspace(c)) {
      return false;
    }
  }

  return true;
}

bool IsGoodFileName(const std::string& filename){
  if (filename.empty()) {
    return false;
  }

  if (filename.length() > 16) {
    return false;
  }

  for (char c : filename) {
    if (!isalnum(c) && !isspace(c) && c != '.' && c != '(' && c != ')') {
      return false;
    }
  }

  return true;
}

std::string ReversePath(const std::string& path) {
  std::stringstream ss(path);
  std::string item;
  std::vector<std::string> components;

  while (std::getline(ss, item, '/')) {
    components.push_back(item);
  }

  std::reverse(components.begin(), components.end());

  std::stringstream reversed_path;
  if (!components.empty()){
    reversed_path << "/" << components[0];
    for (size_t i = 1; i < components.size() - 1; ++i) {
      reversed_path << "/" << components[i];
    }
  }
  return reversed_path.str();
}

}