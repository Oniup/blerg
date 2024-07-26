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

#ifndef IGNITE_CORE_STRINGS__STRING_H
#define IGNITE_CORE_STRINGS__STRING_H

#include "core/containers/iterator.h"
#include "core/limits.h"
#include "core/strings/string_view.h"
#include <cstring>
#include <ctype.h>

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

    constexpr IgString(const IgStringView& view)
          : Size(view.Size)
    {
        Allocator.Allocate(view.Size);
        Ignite::CopyItems(Begin(), End(), view.Data());
        Allocator[Size] = '\0';
    }

    constexpr IgString(const char* str)
          : IgString(IgStringViewCast<const char*>::Cast(str))
    {
    }

    constexpr IgString(const IgString& str)
          : Size(str.Size)
    {
        Allocator.Allocate(str.Allocator.Capacity);
        Ignite::CopyItems(Begin(), End(), str.Allocator.Ptr);
        Allocator[Size] = '\0';
    }

    constexpr IgString(IgString&& str)
          : Size(Ignite::Move(str.Size)), Allocator(Ignite::Move(str.Allocator))
    {
        str.Size = 0;
        str.Allocator.SetToNullptr();
    }

    ~IgString() { Destroy(); }

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

    constexpr size_t Capacity() const { return Allocator.Capacity; }
    constexpr char* Data() { return Allocator.Ptr; }
    constexpr const char* Data() const { return Allocator.Ptr; }
    constexpr const char* CStr() const { return Allocator.Ptr; }

    // constexpr bool IsEmpty() const { return Allocator.IsEmpty() || (Size == 0 || Size == NoPos);
    // }
    constexpr bool IsEmpty() const { return Allocator.IsEmpty(); }
    constexpr bool IsAllocatorEmpty() const { return Allocator.IsEmpty(); }

    constexpr char& operator[](size_t index) { return Allocator.Ptr[index]; }
    constexpr const char& operator[](size_t index) const { return Allocator.Ptr[index]; }

    constexpr IgString& operator=(const IgStringView& view)
    {
        Size = view.Size;
        Allocator.Allocate(view.Size);
        Ignite::CopyItems(Begin(), End(), view.Data());
        Allocator[Size] = '\0';
    }

    constexpr IgString& operator=(const char* str)
    {
        return *this = IgStringViewCast<const char*>::Cast(str);
    }

    constexpr IgString& operator=(const IgString& str)
    {
        Size = str.Size;
        Allocator.Allocate(str.Allocator.Capacity);
        Ignite::CopyItems(Begin(), End(), str.Allocator.Ptr);
        Allocator[Size] = '\0';
        return *this;
    }

    constexpr IgString& operator=(IgString&& str)
    {
        Size      = Ignite::Move(str.Size);
        Allocator = Ignite::Move(str.Allocator);

        str.Size = 0;
        str.Allocator.SetToNullptr();
        return *this;
    }

    constexpr bool operator!=(const IgString& str) const { return !(*this == str); }
    constexpr bool operator==(const IgString& str) const
    {
        if (Size != str.Size) {
            return false;
        }
        for (size_t i = 0; i < Size; i++) {
            if (Allocator[i] != str.Allocator[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr bool operator!=(const IgStringView& view) const { return !(*this == view); }
    constexpr bool operator==(const IgStringView& view) const
    {
        return IgStringViewCast<IgString>::Cast(*this).Compare(view);
    }

    constexpr bool operator!=(const char* cstr) const { return !(*this == cstr); }
    constexpr bool operator==(const char* cstr) const
    {
        return IgStringViewCast<IgString>::Cast(*this).Compare(
            IgStringView(cstr, Ignite::CStrLength(cstr)));
    }

    constexpr void Clear()
    {
        Size             = 0;
        Allocator.Ptr[0] = '\0';
    }

    constexpr void Destroy()
    {
        Size = 0;
        if (Allocator.Ptr != nullptr) {
            Allocator.Free();
        }
    }

    constexpr void Resize(size_t size)
    {
        if (size > Allocator.Capacity) {
            size_t capacity =
                Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
            Allocator.ReAllocate(capacity);
            Allocator.Ptr[Size] = '\0';
        }
        Size = size;
    }

    constexpr void Reserve(size_t capacity)
    {
        Allocator.ReAllocate(capacity);
        if (capacity < Size) {
            Size                = capacity;
            Allocator.Ptr[Size] = '\0';
        }
    }

    constexpr void Append(const IgString& str)
    {
        size_t oldSize = Size;
        Resize(Size + str.Size);
        std::strncpy(Allocator.Ptr + oldSize, str.Allocator.Ptr, str.Size);
        Allocator[Size] = '\0';
    }

    constexpr void Append(const IgStringView& str)
    {
        size_t oldSize = Size;
        Resize(Size + str.Size);
        std::strncpy(Allocator.Ptr + oldSize, str.Ptr, str.Size);
        Allocator[Size] = '\0';
    }

    constexpr void Insert(Iterator pos, const IgString str)
    {
        size_t oldSize = Size;
        Resize(oldSize + str.Size);
        std::memmove(pos.Ptr() + str.Size, pos.Ptr(), str.Size);
        std::strncpy(pos.Ptr(), str.Allocator.Ptr, str.Size);
        Allocator[Size] = '\0';
    }

    constexpr void Insert(Iterator pos, const IgStringView& str)
    {
        size_t oldSize = Size;
        Resize(oldSize + str.Size);
        std::memmove(pos.Ptr() + str.Size, pos.Ptr(), str.Size);
        std::strncpy(pos.Ptr(), str.Ptr, str.Size);
        Allocator[Size] = '\0';
    }

    constexpr void Insert(Iterator pos, const char* str)
    {
        Insert(pos, IgStringViewCast<const char*>::Cast(str));
    }

    constexpr void PopBack(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (size >= Size) {
            Clear();
            return;
        }
        Resize(Size - size);
        Allocator[Size] = '\0';
    }

    constexpr void PopFront(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (size >= Size) {
            Clear();
            return;
        }
        size_t newSize = Size - size;
        std::memmove(Allocator.Ptr, Allocator.Ptr + size, newSize);
        Resize(newSize);
        Allocator[Size] = '\0';
    }

    constexpr void Erase(Iterator begin, Iterator end)
    {
        size_t size = end.Ptr() - begin.Ptr();
        if (size == 0) {
            return;
        }
        if (size >= Size) {
            Clear();
            return;
        }
        size_t newSize = Size - size;
        std::memmove(begin.Ptr(), end.Ptr(), newSize);
        Resize(newSize);
        Allocator[Size] = '\0';
    }

    constexpr IgStringView Slice(size_t offset, size_t size = NoPos)
    {
        if (size == NoPos) {
            size = Size - offset;
        }
        return IgStringView(Allocator.Ptr + offset, size);
    }

    constexpr IgStringView Slice(Iterator begin, Iterator end)
    {
        return IgStringView(begin.Ptr(), end.Ptr() - begin.Ptr());
    }

    constexpr const IgStringView Slice(size_t offset, size_t size = NoPos) const
    {
        if (size == NoPos) {
            size = Size - offset;
        }
        return IgStringView(Allocator.Ptr + offset, size);
    }

    constexpr const IgStringView Slice(ConstIterator begin, ConstIterator end) const
    {
        return IgStringView(begin.Ptr(), end.Ptr() - begin.Ptr());
    }

    constexpr IgString CopySlice(size_t offset, size_t size = NoPos) const
    {
        return IgString(Slice(offset, size));
    }

    constexpr IgString CopySlice(ConstIterator begin, ConstIterator end) const
    {
        return IgString(Slice(begin.Ptr(), end.Ptr() - begin.Ptr()));
    }

    constexpr void WriteToBuffer(char* buffer, size_t maxSize) const
    {
        std::strncpy(buffer, Allocator.Ptr, Ignite::Clamp<size_t>(Size, 0, maxSize));
    }

    constexpr void ToUpper()
    {
        for (size_t i = 0; i < Size; i++) {
            Allocator[i] = (char)toupper(Allocator[i]);
        }
    }

    constexpr void ToLower()
    {
        for (size_t i = 0; i < Size; i++) {
            Allocator[i] = (char)tolower(Allocator[i]);
        }
    }

    constexpr void ToCamelCase()
    {
        bool first = true;
        for (size_t i = 0; i < Size; i++) {
            if (Allocator[i] == ' ' || Allocator[i] == '-' || Allocator[i] == '_') {
                Iterator offset = begin() + i;
                Erase(offset, offset + 1);
                if (!first) {
                    Allocator[i] = (char)toupper(Allocator[i]);
                }
            }
            else {
                first        = false;
                Allocator[i] = (char)tolower(Allocator[i]);
            }
        }
    }

    constexpr void ToPascalCase()
    {
        bool first = true;
        for (size_t i = 0; i < Size; i++) {
            if (first) {
                Allocator[i] = (char)toupper(Allocator[i]);
                first        = false;
            }
            else if (Allocator[i] == ' ' || Allocator[i] == '-' || Allocator[i] == '_') {
                Iterator offset = begin() + i;
                Erase(offset, offset + 1);
                Allocator[i] = (char)toupper(Allocator[i]);
            }
            else {
                Allocator[i] = (char)tolower(Allocator[i]);
            }
        }
    }

    constexpr void Trim(char trim = ' ')
    {
        if (IsEmpty()) {
            return;
        }
        size_t prefix        = 0;
        size_t suffix        = 0;
        bool counting_prefix = true;
        for (size_t i = 0; i < Size; i++) {
            if (counting_prefix) {
                if (Allocator[i] != trim) {
                    counting_prefix = false;
                }
                else {
                    prefix++;
                }
            }
            else {
                if (Allocator[i] == trim) {
                    suffix++;
                }
                else {
                    suffix = 0;
                }
            }
        }
        PopBack(suffix);
        PopFront(prefix);
    }

    constexpr void TrimTrailing(char trim = ' ')
    {
        size_t count = 0;
        for (size_t i = 0; i < Size; i++) {
            if (Allocator[i] == trim) {
                count++;
            }
            else {
                count = 0;
            }
        }
        PopBack(count);
    }

    constexpr size_t Find(const IgString& str) const
    {
        return Find(IgStringView(str.Allocator.Ptr, str.Size));
    }

    constexpr size_t Find(const IgStringView& str) const
    {
        return IgStringView(Allocator.Ptr, Size).Find(str);
    }

    constexpr size_t Find(const char* str) const { return Find(IgStringView(str)); }

    constexpr size_t FindLast(const IgString& str) const
    {
        return FindLast(IgStringView(str.Allocator.Ptr, str.Size));
    }

    constexpr size_t FindLast(const IgStringView& str) const
    {
        return IgStringView(Allocator.Ptr, Size).FindLast(str);
    }

    constexpr size_t FindLast(const char* str) const { return FindLast(IgStringView(str)); }

    template <typename _ArrayAllocator>
    constexpr void FindAll(IgArray<size_t, _ArrayAllocator>& buffer, const IgString& str) const
    {
        FindAll(buffer, IgStringView(str.Allocator.Ptr, str.Size));
    }

    template <typename _ArrayAllocator>
    constexpr void FindAll(IgArray<size_t, _ArrayAllocator>& buffer, const IgStringView& str) const
    {
        IgStringView(Allocator.Ptr, Size).FindAll(buffer, str);
    }

    template <typename _ArrayAllocator>
    constexpr void FindAll(IgArray<size_t, _ArrayAllocator>& buffer, const char* str) const
    {
        FindAll(buffer, IgStringView(str));
    }

    constexpr void Replace(const IgString& search, const IgString& replace)
    {
        Replace(IgStringView(search.Allocator.Ptr, search.Size),
                IgStringView(replace.Allocator.Ptr, replace.Size));
    }

    constexpr bool ReplaceFirst(const IgString& search, const IgString& replace)
    {
        return ReplaceFirst(IgStringView(search.Allocator.Ptr, search.Size),
                            IgStringView(replace.Allocator.Ptr, replace.Size));
    }

    constexpr bool ReplaceLast(const IgString& search, const IgString& replace)
    {
        return ReplaceLast(IgStringView(search.Allocator.Ptr, search.Size),
                           IgStringView(replace.Allocator.Ptr, replace.Size));
    }

    constexpr void Replace(const IgStringView& search, const IgStringView& replace)
    {
        bool next = true;
        while (next) {
            next = ReplaceFirst(search, replace);
        }
    }

    constexpr bool ReplaceFirst(const IgStringView& search, const IgStringView& replace)
    {
        size_t offset = Find(search);
        return _ReplaceInstance(offset, search.Size, replace);
    }

    constexpr bool ReplaceLast(const IgStringView& search, const IgStringView& replace)
    {
        size_t offset = FindLast(search);
        return _ReplaceInstance(offset, search.Size, replace);
    }

    constexpr void Replace(const char* search, const char* replace)
    {
        Replace(IgStringView(search), IgStringView(replace));
    }

    constexpr bool ReplaceFirst(const char* search, const char* replace)
    {

        return ReplaceFirst(IgStringView(search), IgStringView(replace));
    }

    constexpr bool ReplaceLast(const char* search, const char* replace)
    {
        return ReplaceLast(IgStringView(search), IgStringView(replace));
    }

private:
    constexpr bool _ReplaceInstance(size_t pos, size_t search_size, const IgStringView& replace)
    {
        if (pos == NoPos) {
            return false;
        }
        Resize(Size - search_size + replace.Size);

        Iterator offset = Begin() + pos;
        std::memmove(offset.Ptr() + replace.Size, offset.Ptr() + search_size,
                     end().Ptr() - (offset.Ptr() + replace.Size));
        std::strncpy(offset.Ptr(), replace.Ptr, replace.Size);
        Allocator[Size] = '\0';
        return true;
    }
};

template <typename _Allocator, size_t _CapacityIncreaseIntervalSize>
struct IgStringViewCast<IgString<_Allocator, _CapacityIncreaseIntervalSize>> {
    static constexpr IgStringView
        Cast(const IgString<_Allocator, _CapacityIncreaseIntervalSize>& str)
    {
        return IgStringView(str.Data(), str.Size);
    }
};

#endif
