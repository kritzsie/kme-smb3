#pragma once

#include <map>
#include <string>
#include <vector>

#include <cstdint>

namespace kme {
  // Shorter aliases for some common types
  using UInt8 = std::uint8_t;
  using SInt8 = std::int8_t;
  using Int8 = SInt8;

  using UInt16 = std::uint16_t;
  using SInt16 = std::int16_t;
  using Int16 = SInt16;

  using UInt32 = std::uint32_t;
  using SInt32 = std::int32_t;
  using Int32 = SInt32;

  using UInt64 = std::uint64_t;
  using SInt64 = std::int64_t;
  using Int64 = SInt64;

  // Less common types
  using Byte = signed char;
  using UShort = unsigned short;
  using UInt = unsigned int;
  using ULong = unsigned long;

  // Composite types
  using StringList = std::vector<std::string>;

  template<typename T>
  using StringTable = std::map<std::string, T>;
}
