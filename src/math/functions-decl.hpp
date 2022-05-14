#pragma once

namespace kme::fp {
template<typename T, typename U>
constexpr U map(T (&f)(T), const U& self);
}
