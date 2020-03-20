#pragma once

#include <physfs.h>

#include <SFML/System.hpp>

#include <string>
#include <vector>

namespace kme::util {
std::vector<char> readFile(std::string);

class FileInputStream : public sf::FileInputStream {
private:
  PHYSFS_File* filehandle = nullptr;

public:
  bool open(std::string);
  virtual sf::Int64 read(void*, sf::Int64) override;
  virtual sf::Int64 seek(sf::Int64) override;
  virtual sf::Int64 tell() override;
  virtual sf::Int64 getSize() override;

  virtual ~FileInputStream() override;
};
}
