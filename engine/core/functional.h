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

#ifndef CORE__FUNCTIONAL_H
#define CORE__FUNCTIONAL_H

#include "core/memory/utils.h"
#include "core/strings/string.h"
#include "core/strings/string_view.h"
#include <cstdint>
#include <cstdio>

#define FNV_HASH64_DEFAULT_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_HASH64_DEFAULT_PRIME 0x00000100000001B3

#define FNV_HASH128_DEFAULT_OFFSET_BASIS 0x6c62272e07bb014262b821756295c58d
#define FNV_HASH128_DEFAULT_PRIME 0x0000000001000000000000000000013B

namespace fiwre {

// Fowler–Noll–Vo (FNV) is a non-cryptographic hash function created by Glenn
// Fowler, Landon Curt Noll, and Kiem-Phong Vo.
//
// NOTE: Read more information where I got the algorithm,
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// - bytes: Byte data used to compute hash value.
// - size: Number of bytes provided.
// - hash: Offset basis (defaults to FNV_HASH64_DEFAULT_OFFSET_BASIS).
// - prime: Number to multiply each byte by (defaults to
// FNV_HASH64_DEFAULT_PRIME). Returns: A 64-bit hash value computed using the
// FNV algorithm.
inline constexpr uint64_t
    calc_fnv_hash(const char* bytes, size_t size,
                  uint64_t hash  = FNV_HASH64_DEFAULT_OFFSET_BASIS,
                  uint64_t prime = FNV_HASH64_DEFAULT_PRIME) {
  for (size_t i = 0; i < size; i++) {
    hash ^= bytes[i];
    hash *= prime;
  }
  return hash;
}

template <typename T>
struct Hash {
  FORCE_INLINE constexpr size_t hash(const T& val) const {
    return fiwre::calc_fnv_hash(fiwre::byte_cast(val), sizeof(T));
  }
};

template <>
struct Hash<StringView> {
  FORCE_INLINE static constexpr uint64_t hash(const StringView& str) {
    return fiwre::calc_fnv_hash(str.data(), str.size());
  }
};

template <typename TAllocator, size_t TCapacityIncreaseIntervalSize>
struct Hash<BasicString<char, TAllocator, TCapacityIncreaseIntervalSize>> {
  FORCE_INLINE static constexpr uint64_t hash(const StringView& str) {
    return fiwre::calc_fnv_hash(str.data(), str.size());
  }
};

template <typename T>
struct Comparator {
  FORCE_INLINE static constexpr bool compare(const T& lhs, const T& rhs) {
    return lhs == rhs;
  }
};

template <>
struct Comparator<const char*> {
  FORCE_INLINE static constexpr bool compare(const char* lhs, const char* rhs) {
    return fiwre::cstr_compare(lhs, rhs, fiwre::cstr_length(lhs));
  }
};

template <>
struct Comparator<char*> {
  FORCE_INLINE static constexpr bool compare(char* lhs, char* rhs) {
    return fiwre::cstr_compare(lhs, rhs, fiwre::cstr_length(lhs));
  }
};

} // namespace fiwre

#endif
