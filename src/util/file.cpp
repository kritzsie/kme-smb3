#include "file.hpp"

#include "../types.hpp"
#include "string.hpp"

#include <stdexcept>

namespace kme::util {
// remove occurrences of "." and collapse occurrences of "*/.." from a path
// NOTE: this is slow and ignores some edge cases
StringList sanitize(const StringList& path) {
  StringList result;
  result.reserve(path.size());
  for (auto it = path.rbegin(); it < path.rend(); ++it) {
    if (*it != ".") {
      std::size_t discard = 0;
      while (*it == "..") {
        ++discard;
        ++it;
      }
      while (discard > 0) {
        --discard;
        ++it;
      }
      result.insert(result.begin(), *it);
    }
  }
  return result;
}

std::string sanitize(const std::string& path) {
  return join(sanitize(split(path, "/")), "/");
}

StringList getFiles(const std::string& path) {
  if (PHYSFS_isInit() == 0) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  StringList files;

  char** rc = PHYSFS_enumerateFiles(sanitize(path).c_str());
  if (rc == nullptr) {
    std::runtime_error ex(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    PHYSFS_freeList(rc);
    throw ex;
  }

  for (char** i = rc; *i != nullptr; ++i) {
    files.push_back(*i);
  }

  PHYSFS_freeList(rc);
  return files;
}

std::vector<char> readFile(const std::string& path) {
  if (PHYSFS_isInit() == 0) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  PHYSFS_File* file = PHYSFS_openRead(sanitize(path).c_str());
  if (file == nullptr) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  std::size_t length = PHYSFS_fileLength(file);
  if (length == 0) {
    return std::vector<char>();
  }

  std::vector<char> data(length);
  PHYSFS_readBytes(file, data.data(), length);
  PHYSFS_close(file);
  return data;
}

FileInputStream::FileInputStream() : sf::FileInputStream(), filehandle(nullptr) {}

FileInputStream::~FileInputStream() {
  close();
}

bool FileInputStream::open(const std::string& path) {
  close();
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

int FileInputStream::close() {
  return PHYSFS_close(filehandle);
}
}
