#pragma once

#include "../types.hpp"

#include <physfs.h>

#include <SFML/System.hpp>

#include <string>
#include <vector>

namespace kme::util {
StringList sanitize(const StringList& path);
std::string sanitize(const std::string& path);

std::vector<char> readFile(std::string path);

class FileInputStream : public sf::FileInputStream {
public:
  FileInputStream();
  virtual ~FileInputStream() override;

  bool open(const std::string& path);
  virtual sf::Int64 read(void* data, sf::Int64 size) override;
  virtual sf::Int64 seek(sf::Int64 position) override;
  virtual sf::Int64 tell() override;
  virtual sf::Int64 getSize() override;

  int close();

private:
  PHYSFS_File* filehandle;
};
}
