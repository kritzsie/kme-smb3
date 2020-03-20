#include "file.hpp"

#include <stdexcept>

namespace kme::util {
std::vector<char> readFile(std::string path) {
  if (PHYSFS_isInit() == 0) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  PHYSFS_File* file = PHYSFS_openRead(path.c_str());
  if (file == nullptr) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  std::size_t size = PHYSFS_fileLength(file);
  if (size == 0) {
    return std::vector<char>();
  }

  char* data = new char[size];
  PHYSFS_readBytes(file, data, size);
  PHYSFS_close(file);
  return std::vector<char>(data, data + size);
}

bool FileInputStream::open(std::string path) {
  if (filehandle) {
    PHYSFS_close(filehandle);
  }

  filehandle = PHYSFS_openRead(path.c_str());
  return filehandle != nullptr;
}

sf::Int64 FileInputStream::read(void* data, sf::Int64 size) {
  return PHYSFS_readBytes(filehandle, data, size);
}

sf::Int64 FileInputStream::seek(sf::Int64 position) {
  return PHYSFS_seek(filehandle, position) ? position : -1;
}

sf::Int64 FileInputStream::tell() {
  return PHYSFS_tell(filehandle);
}

sf::Int64 FileInputStream::getSize() {
  return PHYSFS_fileLength(filehandle);
}

FileInputStream::~FileInputStream() {
  PHYSFS_close(filehandle);
}
}
