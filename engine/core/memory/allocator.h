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

#ifndef IGNITE_CORE_MEMORY__ALLOCATOR_H
#define IGNITE_CORE_MEMORY__ALLOCATOR_H

#include "core/defines.h"
#include "math/math.h"
#include <cstdio>
#include <cstdlib>

struct IgHeapAllocation {
    template <typename _T>
    struct Allocation {
        using Type = _T;

        size_t Capacity = 0;
        _T* Ptr         = nullptr;

        IG_FORCE_INLINE _T& operator*() const { return *Ptr; }
        IG_FORCE_INLINE _T& operator[](size_t index) { return Ptr[index]; }
        IG_FORCE_INLINE const _T& operator[](size_t index) const { return Ptr[index]; }

        IG_FORCE_INLINE constexpr size_t CalcRequiredCapacitySize(size_t fit_size,
                                                                  size_t capacity_interval) const
        {
            size_t interval_count = (size_t)Ignite::RoundUp((double)fit_size / capacity_interval);
            if (interval_count > 0) {
                return capacity_interval * interval_count;
            }
            return fit_size;
        }

        IG_FORCE_INLINE void Allocate(size_t capacity)
        {
            IG_BASIC_ASSERT(Ptr != nullptr,
                            "Allocation already exists! This will result in a memory leak!!!");
            if (capacity > 0) {
                Ptr = static_cast<_T*>(IG_ALLOCATE_HEAP_MEMORY(capacity * sizeof(_T)));
                IG_BASIC_ASSERT(Ptr == nullptr, "Failed to allocate heap memory");
                Capacity = capacity;
            }
        }

        IG_FORCE_INLINE void ReAllocate(size_t capacity)
        {
            if (capacity == 0) {
                Free();
                return;
            }
            if (Ptr == nullptr) {
                Allocate(capacity);
                return;
            }
            Ptr = static_cast<_T*>(IG_REALLOCATE_HEAP_MEMORY(Ptr, capacity * sizeof(_T)));
            IG_BASIC_ASSERT(Ptr == nullptr, "Failed to reallocate heap memory");
            Capacity = capacity;
        }

        IG_FORCE_INLINE void Free()
        {
            if (Ptr != nullptr) {
                IG_FREE_HEAP_MEMORY(Ptr);
                SetToNullptr();
            }
        }

        IG_FORCE_INLINE bool IsEmpty() const { return Ptr == nullptr; }

        IG_FORCE_INLINE void SetToNullptr()
        {
            Ptr      = nullptr;
            Capacity = 0;
        }
    };
};

template <size_t _Capacity>
struct IgFixedAllocation {
    template <typename _T>
    struct Allocation {
        using Type                       = _T;
        static constexpr size_t Capacity = _Capacity;

        _T Ptr[Capacity];

        IG_FORCE_INLINE constexpr _T& operator*() const { return *Ptr; }
        IG_FORCE_INLINE constexpr _T& operator[](size_t index) { return Ptr[index]; }
        IG_FORCE_INLINE constexpr const _T& operator[](size_t index) const { return Ptr[index]; }

        IG_FORCE_INLINE constexpr size_t CalcRequiredCapacitySize(size_t fit_size,
                                                                  size_t cap_interval_size) const
        {
            size_t interval_count = (size_t)Ignite::RoundUp((double)fit_size / cap_interval_size);
            size_t size           = cap_interval_size * interval_count;
            IG_BASIC_ASSERT(size > Capacity, "Cannot increase Fixed Allocation");
            return size;
        }

        IG_FORCE_INLINE constexpr void Allocate(size_t size)
        {
            IG_BASIC_ASSERT(size > Capacity,
                            "Cannot allocate more than the capacity in a fixed allocator data");
        }

        IG_FORCE_INLINE constexpr void ReAllocate(size_t size)
        {
            IG_BASIC_ASSERT(size > Capacity, "Cannot resize fixed allocator data");
        }

        IG_FORCE_INLINE constexpr bool IsEmpty() const { return false; }
        IG_FORCE_INLINE constexpr void Free() {}
        IG_FORCE_INLINE constexpr void SetToNullptr() {}
    };
};

#endif
