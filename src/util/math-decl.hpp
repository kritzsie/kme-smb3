#pragma once

namespace kme::util {
template<typename T>
constexpr T absdiv(const T& lhs, const T& rhs);

template<typename T>
constexpr T absmod(const T& lhs, const T& rhs);

template<typename T>
constexpr T round(const T& value);
}
