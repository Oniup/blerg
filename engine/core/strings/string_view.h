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

#ifndef IGNITE_CORE_STRINGS__STRING_VIEW_H
#define IGNITE_CORE_STRINGS__STRING_VIEW_H

#include "core/common.h"
#include "core/containers/array.h"
#include "core/containers/iterator.h"
#include "core/limits.h"
#include "core/strings/cstr_utils.h"

template <typename _T>
struct IgStringViewCast {};

struct IgStringView {
    using ConstIterator           = IgConstPackedIterator<char, IgStringView>;
    static constexpr size_t NoPos = IgNumericLimits<size_t>::Max();

    size_t Size     = 0;
    const char* Ptr = nullptr;

    constexpr IgStringView() = default;
    constexpr IgStringView(const char* cstr, size_t size)
          : Size(size), Ptr(cstr)
    {
    }

    constexpr IgStringView(const char* cstr)
          : Size(Ignite::CStrLength(cstr)), Ptr(cstr)
    {
    }

    template <typename _StrT>
    constexpr IgStringView(const _StrT& str)
          : IgStringView(IgStringViewCast<_StrT>::Cast(str))
    {
    }

    constexpr IgStringView(const IgStringView& view)
          : Size(view.Size), Ptr(view.Ptr)
    {
    }

    constexpr IgStringView(IgStringView&& view)
          : Size(view.Size), Ptr(view.Ptr)
    {
        view.Size = 0;
        view.Ptr  = nullptr;
    }

    constexpr ConstIterator Begin() const
    {
        return ConstIterator(const_cast<IgStringView*>(this), 0);
    }

    constexpr ConstIterator End() const
    {
        return ConstIterator(const_cast<IgStringView*>(this), Size);
    }

    constexpr ConstIterator begin() const { return Begin(); }
    constexpr ConstIterator end() const { return End(); }

    constexpr const char* Data() const { return Ptr; }

    constexpr const char& At(size_t index) const { return Ptr[index]; }
    constexpr const char& First() const { return Ptr[0]; }
    constexpr const char& Last() const { return Ptr[Size - 1]; }

    inline constexpr bool IsEmpty() const { return Ptr == nullptr; }
    inline constexpr bool IsNullTerminated() const { return Size == 0 || Ptr[Size] == '\0'; }

    constexpr char operator[](size_t offset) const { return Ptr[offset]; }
    constexpr bool operator==(const IgStringView& view) const { return Compare(view); }
    constexpr bool operator!=(const IgStringView& view) const { return !Compare(view); }

    template <typename _StrT>
    inline constexpr bool operator==(const _StrT& str) const
    {
        return IgStringViewCast<_StrT>::Cast(str) == *this;
    }

    template <typename _StrT>
    inline constexpr bool operator!=(const _StrT& str) const
    {
        return IgStringViewCast<_StrT>::Cast(str) != *this;
    }

    constexpr IgStringView& operator=(const char* str)
    {
        Ptr  = str;
        Size = Ignite::CStrLength(str);
        return *this;
    }

    constexpr IgStringView& operator=(const IgStringView& str)
    {
        Ptr  = str.Ptr;
        Size = str.Size;
        return *this;
    }

    constexpr IgStringView& operator=(IgStringView&& str)
    {
        Ptr  = str.Ptr;
        Size = str.Size;

        str.Ptr  = nullptr;
        str.Size = 0;
        return *this;
    }

    template <typename _StrT>
    inline constexpr IgStringView& operator=(const _StrT& str)
    {
        *this = IgStringViewCast<_StrT>::Cast(str);
        return *this;
    }

    constexpr IgStringView Slice(size_t offset, size_t size = NoPos) const
    {
        if (size == NoPos) {
            size = Size - offset;
        }
        return IgStringView(Ptr + offset, size);
    }

    constexpr void Set(const IgStringView& str)
    {
        Ptr  = str.Ptr;
        Size = str.Size;
    }

    constexpr bool Compare(const IgStringView& str) const
    {
        if (str.Size != Size) {
            return false;
        }
        return Ignite::CStrCompare(Ptr, str.Ptr, Size);
    }

    constexpr size_t Find(const IgStringView& match) const
    {
        for (size_t i = 0; i < Size; i++) {
            if (match.Size > Size - i) {
                return NoPos;
            }
            else if (Slice(i, match.Size) == match) {
                return i;
            }
        }
        return NoPos;
    }

    constexpr size_t FindLast(const IgStringView& match) const
    {
        for (size_t i = Size - match.Size; i >= 0; i--) {
            if (match.Size > Size - i) {
                return NoPos;
            }
            else if (Slice(i, match.Size) == match) {
                return i;
            }
        }
        return NoPos;
    }

    template <typename _ArrayAllocator>
    constexpr void FindAll(IgArray<size_t, _ArrayAllocator>& buffer,
                           const IgStringView& match) const
    {
        if (!(match.Size > 0)) {
            return;
        }
        for (size_t i = 0; i < Size; i++) {
            if (match.Size > Size - i) {
                break;
            }
            else if (Slice(i, match.Size) == match) {
                buffer.PushBack(i);
            }
        }
    }
};

template <>
struct IgStringViewCast<IgStringView> {
    static constexpr IgStringView Cast(const IgStringView& view) { return view; }
};

template <>
struct IgStringViewCast<const char*> {
    static constexpr IgStringView Cast(const char* cstr)
    {
        return IgStringView(cstr, Ignite::CStrLength(cstr));
    }
};

template <>
struct IgStringViewCast<char*> {
    static constexpr IgStringView Cast(char* cstr)
    {
        return IgStringView(cstr, Ignite::CStrLength(cstr));
    }
};

template <size_t _Size>
struct IgStringViewCast<char[_Size]> {
    static constexpr IgStringView Cast(char (&cstr)[_Size])
    {
        return IgStringView(cstr, Ignite::CStrLength(cstr));
    }
};
template <size_t _Size>
struct IgStringViewCast<const char[_Size]> {
    static constexpr IgStringView Cast(const char (&cstr)[_Size])
    {
        return IgStringView(cstr, Ignite::CStrLength(cstr));
    }
};

#endif
