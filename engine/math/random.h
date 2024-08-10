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

#ifndef MATH__RANDOM_H
#define MATH__RANDOM_H

#include <cstdint>
#include <cstdlib>

#define PI 3.14159265358979323846f

#define INTERNAL_IMAX_BITS(m) \
  ((m) / ((m) % 255 + 1) / 255 % 255 * 8 + 7 - 86 / ((m) % 255 + 12))
#define INTERNAL_RAND_MAX_WIDTH INTERNAL_IMAX_BITS(RAND_MAX)
static_assert((RAND_MAX & (RAND_MAX + 1u)) == 0,
              "RAND_MAX not a Mersenne number");

namespace fiwre {

inline uint64_t random_int64() {
  // https://stackoverflow.com/a/33021408
  uint64_t val = 0;
  for (int i = 0; i < 64; i += INTERNAL_RAND_MAX_WIDTH) {
    val <<= INTERNAL_RAND_MAX_WIDTH;
    val ^= (unsigned)rand();
  }
  return val;
}

}  // namespace fiwre

#endif
