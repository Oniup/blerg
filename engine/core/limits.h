// This file is part of Ignite Engine (https://github.com/Oniup/ignite_engine)
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

#ifndef IGNITE_CORE__LIMITS_H
#define IGNITE_CORE__LIMITS_H

#include <cfloat>
#include <climits>
#include <cstdint>
#include <cstdio>

template <typename _Primitive>
struct IgNumericLimits {
    static constexpr _Primitive Max();
    static constexpr _Primitive Min();
};

template <>
struct IgNumericLimits<int8_t> {
    static constexpr int8_t Max() { return (int8_t)INT8_MAX; }
    static constexpr int8_t Min() { return (int8_t)INT8_MIN; }
};

template <>
struct IgNumericLimits<int16_t> {
    static constexpr int16_t Max() { return (int16_t)INT16_MAX; }
    static constexpr int16_t Min() { return (int16_t)INT16_MIN; }
};

template <>
struct IgNumericLimits<int32_t> {
    static constexpr int32_t Max() { return (int32_t)INT32_MAX; }
    static constexpr int32_t Min() { return (int32_t)INT32_MIN; }
};

template <>
struct IgNumericLimits<int64_t> {
    static constexpr int64_t Max() { return (int64_t)INT64_MAX; }
    static constexpr int64_t Min() { return (int64_t)INT64_MIN; }
};

template <>
struct IgNumericLimits<uint8_t> {
    static constexpr uint8_t Max() { return (uint8_t)UINT8_MAX; }
    static constexpr uint8_t Min() { return 0; }
};

template <>
struct IgNumericLimits<uint16_t> {
    static constexpr uint16_t Max() { return (uint16_t)UINT16_MAX; }
    static constexpr uint16_t Min() { return 0; }
};

template <>
struct IgNumericLimits<uint32_t> {
    static constexpr uint32_t Max() { return (uint32_t)UINT32_MAX; }
    static constexpr uint32_t Min() { return 0; }
};

template <>
struct IgNumericLimits<uint64_t> {
    static constexpr uint64_t Max() { return (uint64_t)UINT64_MAX; }
    static constexpr uint64_t Min() { return 0; }
};

template <>
struct IgNumericLimits<float> {
    static constexpr float Max() { return (float)FLT_MAX; }
    static constexpr float Min() { return (float)FLT_MIN; }
};

template <>
struct IgNumericLimits<double> {
    static constexpr double Max() { return (double)DBL_MAX; }
    static constexpr double Min() { return (double)DBL_MIN; }
};

#endif
