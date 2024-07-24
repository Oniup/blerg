// This file is part of Ignite Engine (https://github.com/Oniup/Ignite)
// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IGNITE_MATH__RANDOM_H
#define IGNITE_MATH__RANDOM_H

#include <cmath>
#include <cstdint>
#include <cstdlib>

#define IG_PI 3.14159265358979323846f

#define IG_INTERNAL_IMAX_BITS(m)                                                                  \
    ((m) / ((m) % 255 + 1) / 255 % 255 * 8 + 7 - 86 / ((m) % 255 + 12))
#define IG_INTERNAL_RAND_MAX_WIDTH IG_INTERNAL_IMAX_BITS(RAND_MAX)
static_assert((RAND_MAX & (RAND_MAX + 1u)) == 0, "RAND_MAX not a Mersenne number");

namespace Ignite {

inline uint64_t RandomUInt64()
{
    // https://stackoverflow.com/a/33021408
    uint64_t val = 0;
    for (int i = 0; i < 64; i += IG_INTERNAL_RAND_MAX_WIDTH) {
        val <<= IG_INTERNAL_RAND_MAX_WIDTH;
        val ^= (unsigned)rand();
    }
    return val;
}

inline constexpr double RoundUp(double val)
{
    constexpr size_t intmax = ((INTMAX_MAX / 2 + 1) * 2.0);
    if (val >= INTMAX_MIN && val < intmax) {
        intmax_t i = (intmax_t)val;
        if (i < 0 || val == i) {
            return i;
        }
        return i + 1.0;
    }
    return val;
}

inline constexpr double RoundDown(double val)
{
    constexpr size_t intmax = ((INTMAX_MAX / 2 + 1) * 2.0);
    if (val >= INTMAX_MIN && val < intmax) {
        intmax_t i = (intmax_t)val;
        if (i > 0 || val == i) {
            return i;
        }
        return i - 1.0;
    }
    return val;
}

template <typename _T>
inline constexpr _T Clamp(_T val, _T min, _T max)
{
    if (val < min) {
        return min;
    }
    else if (val > max) {
        return max;
    }
    return val;
}

template <typename _T>
inline constexpr _T Pow(_T val, _T exp)
{
    _T result = 1;
    for (_T i = 0; i < exp; i++) {
        result *= val;
    }
    return result;
}

template <typename _T>
inline constexpr _T Pow2(_T val)
{
    return val * val;
}

template <typename _T>
inline constexpr _T Learp(_T val, _T dest, _T time)
{
    return val + (dest - val) * time;
}

} // namespace Ignite

#endif
