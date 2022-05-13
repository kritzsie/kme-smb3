#pragma once

#include "../types.hpp"

#include <type_traits>

namespace kme {
// Strongly-typed sign scalar
enum class Sign { PLUS, MINUS };

Sign operator +(const Sign& rhs);
Sign operator -(const Sign& rhs);

template<typename T,
  std::enable_if_t<not std::is_same_v<Sign, T>, std::nullptr_t> = nullptr>
T operator *(const T& lhs, const Sign& rhs);

template<typename T>
T operator *(const Sign& lhs, const T& rhs);

template<typename T>
Sign toSign(const T& value);

// Wide types
// These can be used to perform range calculations such that, if x and y are of
// some type T, then typename Widen<T>::type is guaranteed to be wide enough to
// contain the result of x - y. For example:
// ```c++
// using WideByte = Widen<Byte>::type;
// Byte x1 = 100, x2 = 50;
// Byte     dx   = x2 - x1; // Byte is not wide enough to represent -50
// WideByte wide = x2 - x1; // But this type is
// ```
template<typename T>
struct Widen {};

template<> struct Widen<bool> { using type = SInt8; };

template<> struct Widen<SInt8>  { using type = SInt16; };
template<> struct Widen<SInt16> { using type = SInt32; };
template<> struct Widen<SInt32> { using type = SInt64; };

template<> struct Widen<UInt8>  { using type = SInt16; };
template<> struct Widen<UInt16> { using type = SInt32; };
template<> struct Widen<UInt32> { using type = SInt64; };

// floating-point types are a mild exception
template<> struct Widen<float>       { using type = float; };
template<> struct Widen<double>      { using type = double; };
template<> struct Widen<long double> { using type = long double; };
}
