#include "string.hpp"

#include "../types.hpp"

#include <regex>
#include <sstream>
#include <string>

namespace kme::util {
char highASCII(char c) {
  return c |= 128;
}

std::string highASCII(std::string str) {
  std::string result = str;
  for (auto& c : result) {
    c = highASCII(c);
  }
  return result;
}

std::string trim(std::string str, std::string pattern) {
  std::string result = str;
  std::regex front("^" + pattern);
  std::regex back(pattern + "$");
  result = std::regex_replace(result, front, "");
  result = std::regex_replace(result, back, "");
  return result;
}

std::string trim(std::string str) {
  return trim(str, "\\s+");
}

StringList split(std::string str, std::string pattern) {
  StringList lst;
  std::regex regex(pattern);
  std::sregex_token_iterator iter(str.begin(), str.end(), regex, -1);
  std::sregex_token_iterator end;
  while (iter != end) {
    lst.push_back(*iter);
    ++iter;
  }
  return lst;
}

StringList split(std::string str) {
  return split(str, "\\s+");
}

std::string join(const StringList& lst, std::string sep) {
  std::stringstream ss;
  if (lst.size()) {
    ss << lst[0];
    for (auto iter = lst.begin() + 1; iter != lst.end(); ++iter) {
      ss << sep << *iter;
    }
  }
  return ss.str();
}

std::string join(const StringList& lst) {
  return join(lst, " ");
}
}
