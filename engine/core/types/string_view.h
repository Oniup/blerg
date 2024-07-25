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

#ifndef IGNITE_CORE_TYPES__STRING_VIEW_H
#define IGNITE_CORE_TYPES__STRING_VIEW_H

#include "core/common.h"
#include "core/cstr_utils.h"
#include "core/limits.h"
#include "core/types/array.h"
#include "core/types/iterator.h"

template <typename _T>
struct IgStringViewCast {
    static constexpr IgStringView Cast(const _T& str);
};

struct IgStringView {
    using ConstIterator           = IgConstPackedIterator<char, IgStringView>;
    static constexpr size_t NoPos = IgNumericLimits<size_t>::Max();

    constexpr IgStringView() = default;
    constexpr IgStringView(const char* cstr, size_t size)
          : m_Size(size), m_Ptr(cstr)
    {
    }

    constexpr IgStringView(const char* cstr)
          : m_Size(Ignite::CStrLength(cstr)), m_Ptr(cstr)
    {
    }

    template <typename _StringType>
    constexpr IgStringView(const _StringType& str)
          : IgStringView(IgStringViewCast<_StringType>::Cast(str))
    {
    }

    constexpr IgStringView(const IgStringView& view)
          : m_Size(view.m_Size), m_Ptr(view.m_Ptr)
    {
    }

    constexpr IgStringView(IgStringView&& view)
          : m_Size(view.m_Size), m_Ptr(view.m_Ptr)
    {
        view.m_Size = 0;
        view.m_Ptr  = nullptr;
    }

    constexpr ConstIterator Begin() const
    {
        return ConstIterator(const_cast<IgStringView*>(this), 0);
    }

    constexpr ConstIterator End() const
    {
        return ConstIterator(const_cast<IgStringView*>(this), m_Size);
    }

    constexpr ConstIterator begin() const { return Begin(); }
    constexpr ConstIterator end() const { return End(); }

    constexpr const char* Data() const { return m_Ptr; }
    constexpr size_t Size() const { return m_Size; }

    constexpr const char& At(size_t index) const { return m_Ptr[index]; }
    constexpr const char& First() const { return m_Ptr[0]; }
    constexpr const char& Last() const { return m_Ptr[m_Size - 1]; }

    inline constexpr bool IsEmpty() const { return m_Ptr == nullptr; }
    inline constexpr bool IsNullTerminated() const { return m_Size == 0 || m_Ptr[m_Size] == '\0'; }

    constexpr char operator[](size_t offset) const { return m_Ptr[offset]; }
    constexpr bool operator==(const IgStringView& view) const { return Compare(view); }
    constexpr bool operator!=(const IgStringView& view) const { return !Compare(view); }

    template <typename _StringType>
    inline constexpr bool operator==(const _StringType& str) const
    {
        return IgStringViewCast<_StringType>::Cast(str) == *this;
    }

    template <typename _StringType>
    inline constexpr bool operator!=(const _StringType& str) const
    {
        return IgStringViewCast<_StringType>::Cast(str) != *this;
    }

    constexpr IgStringView& operator=(const char* str)
    {
        m_Ptr  = str;
        m_Size = Ignite::CStrLength(str);
        return *this;
    }

    constexpr IgStringView& operator=(const IgStringView& str)
    {
        m_Ptr  = str.m_Ptr;
        m_Size = str.m_Size;
        return *this;
    }

    constexpr IgStringView& operator=(IgStringView&& str)
    {
        m_Ptr  = str.m_Ptr;
        m_Size = str.m_Size;

        str.m_Ptr  = nullptr;
        str.m_Size = 0;
        return *this;
    }

    template <typename _StringType>
    inline constexpr IgStringView& operator=(const _StringType& str)
    {
        *this = IgStringViewCast<_StringType>::Cast(str);
        return *this;
    }

    constexpr IgStringView Slice(size_t offset, size_t size = NoPos) const
    {
        if (size == NoPos) {
            size = m_Size - offset;
        }
        return IgStringView(m_Ptr + offset, size);
    }

    constexpr void Set(const IgStringView& str)
    {
        m_Ptr  = str.m_Ptr;
        m_Size = str.m_Size;
    }

    constexpr bool Compare(const IgStringView& str) const
    {
        if (str.m_Size != m_Size) {
            return false;
        }
        return Ignite::CStrCompare(m_Ptr, str.m_Ptr, m_Size);
    }

    constexpr size_t Find(const IgStringView& match) const
    {
        for (size_t i = 0; i < m_Size; i++) {
            if (match.m_Size > m_Size - i) {
                return NoPos;
            }
            else if (Slice(i, match.m_Size) == match) {
                return i;
            }
        }
        return NoPos;
    }

    constexpr size_t FindLast(const IgStringView& match) const
    {
        for (size_t i = m_Size - match.m_Size; i >= 0; i--) {
            if (match.m_Size > m_Size - i) {
                return NoPos;
            }
            else if (Slice(i, match.m_Size) == match) {
                return i;
            }
        }
        return NoPos;
    }

    IG_FORCE_INLINE IgArray<size_t> FindAll(const IgStringView& match) const
    {
        IgArray<size_t> matches;
        if (!(match.m_Size > 0)) {
            return matches;
        }
        for (size_t i = 0; i < m_Size; i++) {
            if (match.m_Size > m_Size - i) {
                break;
            }
            else if (Slice(i, match.m_Size) == match) {
                matches.PushBack(i);
            }
        }
        return matches;
    }

private:
    size_t m_Size     = 0;
    const char* m_Ptr = nullptr;
};

template <>
struct IgStringViewCast<const char> {
    static constexpr IgStringView Cast(const char* const& cstr)
    {
        return IgStringView(cstr, Ignite::CStrLength(cstr));
    }
};

template <>
struct IgStringViewCast<char> {
    static constexpr IgStringView Cast(char* const& cstr)
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
