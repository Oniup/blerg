// This file is part of Fiwre (https://github.com/oniup/fiwre)
// Copyright (c) 2024 Oniup (https://github.com/oniup)
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

#ifndef CORE__LIMITS_H
#define CORE__LIMITS_H

#include <cfloat>
#include <climits>
#include <cstdint>
#include <cstdio>

namespace fiwre {

template <typename _Primitive>
struct NumericLimits {
  static constexpr _Primitive max();
  static constexpr _Primitive min();
};

template <>
struct NumericLimits<int8_t> {
  static constexpr int8_t max() { return (int8_t)INT8_MAX; }
  static constexpr int8_t min() { return (int8_t)INT8_MIN; }
};

template <>
struct NumericLimits<int16_t> {
  static constexpr int16_t max() { return (int16_t)INT16_MAX; }
  static constexpr int16_t min() { return (int16_t)INT16_MIN; }
};

template <>
struct NumericLimits<int32_t> {
  static constexpr int32_t max() { return (int32_t)INT32_MAX; }
  static constexpr int32_t min() { return (int32_t)INT32_MIN; }
};

template <>
struct NumericLimits<int64_t> {
  static constexpr int64_t max() { return (int64_t)INT64_MAX; }
  static constexpr int64_t min() { return (int64_t)INT64_MIN; }
};

template <>
struct NumericLimits<uint8_t> {
  static constexpr uint8_t max() { return (uint8_t)UINT8_MAX; }
  static constexpr uint8_t min() { return 0; }
};

template <>
struct NumericLimits<uint16_t> {
  static constexpr uint16_t max() { return (uint16_t)UINT16_MAX; }
  static constexpr uint16_t min() { return 0; }
};

template <>
struct NumericLimits<uint32_t> {
  static constexpr uint32_t max() { return (uint32_t)UINT32_MAX; }
  static constexpr uint32_t min() { return 0; }
};

template <>
struct NumericLimits<uint64_t> {
  static constexpr uint64_t max() { return (uint64_t)UINT64_MAX; }
  static constexpr uint64_t min() { return 0; }
};

template <>
struct NumericLimits<float> {
  static constexpr float max() { return (float)FLT_MAX; }
  static constexpr float min() { return (float)FLT_MIN; }
};

template <>
struct NumericLimits<double> {
  static constexpr double max() { return (double)DBL_MAX; }
  static constexpr double min() { return (double)DBL_MIN; }
};

} // namespace fiwre

#endif
