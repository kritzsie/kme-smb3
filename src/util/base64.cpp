#include "base64.hpp"

#include <string>
#include <vector>

namespace kme::util {
static constexpr char base64_chars[] = \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::vector<char> base64_decode(std::string str) {
  std::vector<char> result;

  std::vector<char> T(256, -1);
  for (std::size_t i = 0; i < 64; i++)
    T[base64_chars[i]] = i;

  int val = 0, valb = -8;
  for (auto c : str) {
    if (T[c] != -1) {
      val = (val << 6) + T[c];
      valb += 6;
      if (valb >= 0) {
        result.push_back((val >> valb) & 0xFF);
        valb -= 8;
      }
    }
  }

  return result;
}

std::string base64_encode(const std::vector<char>& data) {
  std::string result;

  int val = 0, valb = -6;
  for (auto c : data) {
    val = (val << 8) + c;
    valb += 8;
    while (valb >= 0) {
      result.push_back(base64_chars[(val >> valb) & 0x3F]);
      valb -= 6;
    }
  }

  if (valb > -6)
    result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);

  while (result.length() % 4)
    result.push_back('=');

  return result;
}
}
