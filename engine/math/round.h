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

#ifndef MATH__ROUND_H
#define MATH__ROUND_H

#include <cmath>
#include <cstdint>

namespace fiwre {

inline constexpr double round_up(double val) {
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

inline constexpr double round_down(double val) {
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

}  // namespace fiwre

#endif
