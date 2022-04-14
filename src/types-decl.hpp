#pragma once

#include <map>
#include <string>
#include <vector>

#include <cmath>
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

// TODO: the following aren't simple type aliases and should be in their

// Wide types
// These can be used to perform range calculations such that, if x and y are of
// some type T, then typename Widen<T>::type is guaranteed to be wide enough to
// contain the results of x - y. For example:
// Byte x = -10;
// Byte y = 120;
// Byte narrow = x - y; // Byte is not large enough to represent -130
// typename Widen<Byte>::type wide = x - y; // But this is
template<typename T>
struct Widen {};

template<> struct Widen<bool> { using type = SInt8; };

template<> struct Widen<SInt8>  { using type = SInt16; };
template<> struct Widen<SInt16> { using type = SInt32; };
template<> struct Widen<SInt32> { using type = SInt64; };
template<> struct Widen<SInt64> { using type = SInt64; };

template<> struct Widen<UInt8>  { using type = SInt16; };
template<> struct Widen<UInt16> { using type = SInt32; };
template<> struct Widen<UInt32> { using type = SInt64; };
template<> struct Widen<UInt64> { using type = SInt64; };

template<> struct Widen<float>       { using type = float; };
template<> struct Widen<double>      { using type = double; };
template<> struct Widen<long double> { using type = long double; };

// Strongly-typed sign scalar
enum class Sign { PLUS, MINUS };

Sign operator +(const Sign& rhs);
Sign operator -(const Sign& rhs);

template<typename T>
T operator *(const Sign& lhs, const T& rhs);

template<typename T>
T operator *(const T& lhs, const Sign& rhs);

template<typename T>
Sign toSign(const T& value);
}
