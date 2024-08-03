// This file is part of Blerg (https://github.com/oniup/blerg)
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

#ifndef CORE_MEMORY__ALLOCATOR_H
#define CORE_MEMORY__ALLOCATOR_H

#include "core/defines.h"
#include "math/math.h"
#include <cstdio>
#include <cstdlib>

namespace blerg {

struct HeapAllocator {
    template <typename T>
    struct Allocation {
        using Type = T;

        size_t capacity = 0;
        T* ptr          = nullptr;

        FORCE_INLINE T& operator*() const { return *ptr; }
        FORCE_INLINE T& operator[](size_t index) { return ptr[index]; }
        FORCE_INLINE const T& operator[](size_t index) const { return ptr[index]; }

        FORCE_INLINE constexpr size_t calc_required_capacity_size(size_t fit_size,
                                                                  size_t capacity_interval) const
        {
            size_t interval_count = (size_t)blerg::round_up((double)fit_size / capacity_interval);
            if (interval_count > 0) {
                return capacity_interval * interval_count;
            }
            return fit_size;
        }

        FORCE_INLINE void allocate(size_t size)
        {
            BASIC_ASSERT(ptr != nullptr,
                         "Allocation already exists! This will result in a memory leak!!!");
            if (size > 0) {
                ptr = static_cast<T*>(ALLOCATE_HEAP_MEMORY(size * sizeof(T)));
                BASIC_ASSERT(ptr == nullptr, "Failed to allocate heap memory");
                capacity = size;
            }
        }

        FORCE_INLINE void reallocate(size_t size)
        {
            if (size == 0) {
                free();
                return;
            }
            if (ptr == nullptr) {
                allocate(size);
                return;
            }
            ptr = static_cast<T*>(REALLOCATE_HEAP_MEMORY(ptr, size * sizeof(T)));
            BASIC_ASSERT(ptr == nullptr, "Failed to reallocate heap memory");
            capacity = size;
        }

        FORCE_INLINE void free()
        {
            if (ptr != nullptr) {
                FREE_HEAP_MEMORY(ptr);
                set_to_nullptr();
            }
        }

        FORCE_INLINE bool is_empty() const { return ptr == nullptr; }

        FORCE_INLINE void set_to_nullptr()
        {
            ptr      = nullptr;
            capacity = 0;
        }
    };
};

template <size_t SCapacity>
struct FixedAllocation {
    template <typename T>
    struct Allocation {
        using Type                       = T;
        static constexpr size_t capacity = SCapacity;

        T ptr[capacity];

        FORCE_INLINE constexpr T& operator*() const { return *ptr; }
        FORCE_INLINE constexpr T& operator[](size_t index) { return ptr[index]; }
        FORCE_INLINE constexpr const T& operator[](size_t index) const { return ptr[index]; }

        FORCE_INLINE constexpr size_t calc_required_capacity_size(size_t fit_size,
                                                                  size_t cap_interval_size) const
        {
            size_t interval_count = (size_t)blerg::round_up((double)fit_size / cap_interval_size);
            size_t size           = cap_interval_size * interval_count;
            BASIC_ASSERT(size > capacity, "Cannot increase Fixed Allocation");
            return size;
        }

        FORCE_INLINE constexpr void allocate(size_t size)
        {
            BASIC_ASSERT(size > capacity,
                         "Cannot allocate more than the capacity in a fixed allocator data");
        }

        FORCE_INLINE constexpr void reallocate(size_t size)
        {
            BASIC_ASSERT(size > capacity, "Cannot resize fixed allocator data");
        }

        FORCE_INLINE constexpr bool is_empty() const { return false; }
        FORCE_INLINE constexpr void free() {}
        FORCE_INLINE constexpr void set_to_nullptr() {}
    };
};

} // namespace blerg

#endif
