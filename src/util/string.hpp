#pragma once

#include "../types.hpp"

#include <string>
#include <vector>

namespace kme::util {
char highASCII(char str);
std::string highASCII(std::string str);

std::string trim(std::string str, std::string pattern);
std::string trim(std::string str);

StringList split(std::string str, std::string pattern);
StringList split(std::string str);

std::string join(const StringList& lst, std::string sep);
std::string join(const StringList& lst);
}
