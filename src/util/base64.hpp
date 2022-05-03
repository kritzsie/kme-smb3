#pragma once

#include <string>
#include <vector>

namespace kme::util {
std::vector<char> base64_decode(std::string str);
std::string base64_encode(const std::vector<char>& data);
}
