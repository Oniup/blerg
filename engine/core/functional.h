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

#ifndef IGNITE_CORE__FUNCTIONAL_H
#define IGNITE_CORE__FUNCTIONAL_H

#include "core/memory/utils.h"
#include "core/strings/string.h"
#include "core/strings/string_view.h"
#include <cstdint>
#include <cstdio>

#define IG_FNV_HASH64_DEFAULT_OFFSET_BASIS 0xcbf29ce484222325
#define IG_FNV_HASH64_DEFAULT_PRIME 0x00000100000001B3

#define IG_FNV_HASH128_DEFAULT_OFFSET_BASIS 0x6c62272e07bb014262b821756295c58d
#define IG_FNV_HASH128_DEFAULT_PRIME 0x0000000001000000000000000000013B

namespace Ignite {

// Fowler–Noll–Vo (FNV) is a non-cryptographic hash function created by Glenn Fowler, Landon
// Curt Noll, and Kiem-Phong Vo.
//
// NOTE: Read more information where I got the algorithm,
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// - bytes: Byte data used to compute hash value.
// - size: Number of bytes provided.
// - hash: Offset basis (defaults to IG_FNV_HASH64_DEFAULT_OFFSET_BASIS).
// - prime: Number to multiply each byte by (defaults to IG_FNV_HASH64_DEFAULT_PRIME).
// Returns: A 64-bit hash value computed using the FNV algorithm.
inline constexpr uint64_t CalcFnvHash(const char* bytes, size_t size,
                                      uint64_t hash  = IG_FNV_HASH64_DEFAULT_OFFSET_BASIS,
                                      uint64_t prime = IG_FNV_HASH64_DEFAULT_PRIME)
{
    for (size_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= prime;
    }
    return hash;
}

} // namespace Ignite

template <typename T>
struct IgHash {
    IG_FORCE_INLINE constexpr size_t Hash(const T& val) const
    {
        return Ignite::CalcFnvHash(Ignite::ByteCast(val), sizeof(T));
    }
};

template <>
struct IgHash<IgStringView> {
    IG_FORCE_INLINE static constexpr uint64_t Hash(const IgStringView& str)
    {
        return Ignite::CalcFnvHash(str.Ptr, str.Size);
    }
};

template <typename TAllocator, size_t TCapacityIncreaseIntervalSize>
struct IgHash<IgString<TAllocator, TCapacityIncreaseIntervalSize>> {
    IG_FORCE_INLINE static constexpr uint64_t Hash(const IgStringView& str)
    {
        return Ignite::CalcFnvHash(str.Ptr, str.Size);
    }
};

template <typename T>
struct IgComparator {
    IG_FORCE_INLINE static constexpr bool Compare(const T& lhs, const T& rhs)
    {
        return lhs == rhs;
    }
};

template <>
struct IgComparator<const char*> {
    IG_FORCE_INLINE static constexpr bool Compare(const char* lhs, const char* rhs)
    {
        return Ignite::CStrCompare(lhs, rhs, Ignite::CStrLength(lhs));
    }
};

template <>
struct IgComparator<char*> {
    IG_FORCE_INLINE static constexpr bool Compare(char* lhs, char* rhs)
    {
        return Ignite::CStrCompare(lhs, rhs, Ignite::CStrLength(lhs));
    }
};

#endif
