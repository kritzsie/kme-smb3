#pragma once

#include "../types.hpp"

#include <string>
#include <vector>

namespace kme::util {
char highASCII(char str);
std::string highASCII(std::string str);

std::string trim(const std::string& str, const std::string& pattern);
std::string trim(const std::string& str);

StringList split(const std::string& str, const std::string& pattern);
StringList split(const std::string& str);

std::string join(const StringList& lst, const std::string& sep);
std::string join(const StringList& lst);
}
