// This file is part of Ignite Engine (https://github.com/Oniup/Ignite)
// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applibasic_stringcable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IGNITE_CORE_TYPES__STRING_H
#define IGNITE_CORE_TYPES__STRING_H

#include "core/limits.h"
#include "core/types/iterator.h"
#include "core/types/string_view.h"

template <typename _Allocator, size_t _CapacityIncreaseIntervalSize>
struct IgString {
    using Allocation    = typename _Allocator::template Allocation<char>;
    using Iterator      = IgPackedIterator<char, IgString>;
    using ConstIterator = IgConstPackedIterator<char, IgString>;
    static constexpr size_t CapacityIncreaseIntervalSize = _CapacityIncreaseIntervalSize;
    static constexpr size_t NoPos                        = IgNumericLimits<size_t>::Max();

    size_t Size          = 0;
    Allocation Allocator = {};

    constexpr IgString() = default;

    constexpr IgString(size_t size)
          : Size(size)
    {
        size_t capacity = Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
        Allocator.Allocate(capacity);
        Allocator.Ptr[size] = '\0';
    }

    constexpr IgString(const char* string) {}
    constexpr IgString(const IgStringView& view) {}

    ~IgString() {}

    constexpr Iterator Begin() { return Iterator(this, 0); }
    constexpr Iterator End() { return Iterator(this, Size); }
    constexpr ConstIterator Begin() const { return ConstIterator(const_cast<IgString*>(this), 0); }
    constexpr ConstIterator End() const
    {
        return ConstIterator(const_cast<IgString*>(this), Size);
    }

    constexpr Iterator begin() { return Begin(); }
    constexpr Iterator end() { return End(); }
    constexpr ConstIterator begin() const { return Begin(); }
    constexpr ConstIterator end() const { return End; }

    constexpr char* PtrData() { return Allocator.Ptr; }
    constexpr const char* PtrData() const { return Allocator.Ptr; }
};

template <typename _Allocator, size_t _CapacityIncreaseIntervalSize>
struct IgStringViewCast<IgString<_Allocator, _CapacityIncreaseIntervalSize>> {
    static constexpr IgStringView
        Cast(const IgString<_Allocator, _CapacityIncreaseIntervalSize>& str)
    {
        return IgStringView(str.PtrData(), str.Size);
    }
};

#endif
