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

#ifndef CORE_STRINGS__STRING_H
#define CORE_STRINGS__STRING_H

#include "core/containers/iterator.h"
#include "core/limits.h"
#include "core/strings/string_view.h"
#include <cstring>
#include <ctype.h>

namespace blerg {

template <typename T, typename TAllocator = HeapAllocator,
          size_t TCapacityIncreaseIntervalSize = DEFAULT_CAPACITY_INTERVAL>
struct BasicString {
    using Allocation    = typename TAllocator::template Allocation<char>;
    using Iterator      = PackedIterator<char, BasicString>;
    using ConstIterator = ConstPackedIterator<char, BasicString>;
    static constexpr size_t CapacityIncreaseIntervalSize = TCapacityIncreaseIntervalSize;
    static constexpr size_t NoPos                        = NumericLimits<size_t>::max();

    constexpr BasicString() = default;

    constexpr BasicString(size_t size)
          : _size(size)
    {
        size_t capacity =
            _allocator.calc_required_capacity_size(size, CapacityIncreaseIntervalSize);
        _allocator.allocate(capacity);
        _allocator.ptr[size] = '\0';
    }

    constexpr BasicString(const StringView& view)
          : _size(view.size())
    {
        _allocator.allocate(view.size());
        blerg::copy_items(begin(), end(), view.data());
        _allocator[_size] = '\0';
    }

    constexpr BasicString(const char* str)
          : BasicString(StringViewCast<const char*>::cast(str))
    {
    }

    constexpr BasicString(const BasicString& str)
          : _size(str._size)
    {
        _allocator.allocate(str._allocator.capacity);
        blerg::copy_items(begin(), end(), str._allocator.ptr);
        _allocator[_size] = '\0';
    }

    constexpr BasicString(BasicString&& str)
          : _size(blerg::move(str._size)), _allocator(blerg::move(str._allocator))
    {
        str._size = 0;
        str._allocator.set_to_nullptr();
    }

    ~BasicString() { destroy(); }

    constexpr Iterator begin() { return Iterator(this, 0); }
    constexpr Iterator end() { return Iterator(this, _size); }
    constexpr ConstIterator begin() const
    {
        return ConstIterator(const_cast<BasicString*>(this), 0);
    }
    constexpr ConstIterator end() const
    {
        return ConstIterator(const_cast<BasicString*>(this), _size);
    }

    constexpr size_t capacity() const { return _allocator.capacity; }
    constexpr char* data() { return _allocator.ptr; }
    constexpr const char* data() const { return _allocator.ptr; }
    constexpr const char* cstr() const { return _allocator.ptr; }
    constexpr size_t size() const { return _size; }

    // constexpr bool is_empty() const { return _allocator.is_empty() || (_size == 0 || _size ==
    // NoPos);
    // }
    constexpr bool is_empty() const { return _allocator.is_empty(); }
    constexpr bool is_allocator_empty() const { return _allocator.is_empty(); }

    constexpr char& operator[](size_t index) { return _allocator.ptr[index]; }
    constexpr const char& operator[](size_t index) const { return _allocator.ptr[index]; }

    constexpr BasicString& operator=(const StringView& view)
    {
        _size = view.size();
        _allocator.allocate(view.size());
        blerg::copy_items(begin(), end(), view.data());
        _allocator[_size] = '\0';
    }

    constexpr BasicString& operator=(const char* str)
    {
        return *this = StringViewCast<const char*>::cast(str);
    }

    constexpr BasicString& operator=(const BasicString& str)
    {
        _size = str._size;
        _allocator.allocate(str._allocator.capacity);
        blerg::copy_items(begin(), end(), str._allocator.ptr);
        _allocator[_size] = '\0';
        return *this;
    }

    constexpr BasicString& operator=(BasicString&& str)
    {
        _size      = blerg::move(str._size);
        _allocator = blerg::move(str._allocator);

        str._size = 0;
        str._allocator.set_to_nullptr();
        return *this;
    }

    constexpr bool operator!=(const BasicString& str) const { return !(*this == str); }
    constexpr bool operator==(const BasicString& str) const
    {
        if (_size != str._size) {
            return false;
        }
        for (size_t i = 0; i < _size; i++) {
            if (_allocator[i] != str._allocator[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr bool operator!=(const StringView& view) const { return !(*this == view); }
    constexpr bool operator==(const StringView& view) const
    {
        return StringViewCast<BasicString>::Cast(*this).Compare(view);
    }

    constexpr bool operator!=(const char* cstr) const { return !(*this == cstr); }
    constexpr bool operator==(const char* cstr) const
    {
        return StringViewCast<BasicString>::Cast(*this).Compare(
            StringView(cstr, blerg::cstr_length(cstr)));
    }

    constexpr void clear()
    {
        _size             = 0;
        _allocator.ptr[0] = '\0';
    }

    constexpr void destroy()
    {
        _size = 0;
        if (_allocator.ptr != nullptr) {
            _allocator.free();
        }
    }

    constexpr void resize(size_t size)
    {
        if (size > _allocator.capacity) {
            size_t capacity =
                _allocator.calc_required_capacity_size(size, CapacityIncreaseIntervalSize);
            _allocator.reallocate(capacity);
            _allocator.ptr[_size] = '\0';
        }
        _size = size;
    }

    constexpr void reserve(size_t capacity)
    {
        _allocator.reallocate(capacity);
        if (capacity < _size) {
            _size                 = capacity;
            _allocator.ptr[_size] = '\0';
        }
    }

    constexpr void append(const BasicString& str)
    {
        size_t oldSize = _size;
        resize(_size + str._size);
        std::strncpy(_allocator.ptr + oldSize, str._allocator.ptr, str._size);
        _allocator[_size] = '\0';
    }

    constexpr void append(const StringView& str)
    {
        size_t oldSize = _size;
        resize(_size + str.size());
        std::strncpy(_allocator.ptr + oldSize, str.data(), str.size());
        _allocator[_size] = '\0';
    }

    constexpr void insert(Iterator pos, const BasicString str)
    {
        size_t oldSize = _size;
        resize(oldSize + str._size);
        std::memmove(pos.ptr() + str._size, pos.ptr(), str._size);
        std::strncpy(pos.ptr(), str._allocator.ptr, str._size);
        _allocator[_size] = '\0';
    }

    constexpr void insert(Iterator pos, const StringView& str)
    {
        size_t oldSize = _size;
        resize(oldSize + str.size());
        std::memmove(pos.ptr() + str.size(), pos.ptr(), str.size());
        std::strncpy(pos.ptr(), str.data(), str.size());
        _allocator[_size] = '\0';
    }

    constexpr void insert(Iterator pos, const char* str)
    {
        insert(pos, StringViewCast<const char*>::cast(str));
    }

    constexpr void pop_back(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (size >= _size) {
            clear();
            return;
        }
        resize(_size - size);
        _allocator[_size] = '\0';
    }

    constexpr void pop_front(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (size >= _size) {
            clear();
            return;
        }
        size_t newSize = _size - size;
        std::memmove(_allocator.ptr, _allocator.ptr + size, newSize);
        resize(newSize);
        _allocator[_size] = '\0';
    }

    constexpr void erase(Iterator begin, Iterator end)
    {
        size_t size = end.ptr() - begin.ptr();
        if (size == 0) {
            return;
        }
        if (size >= _size) {
            clear();
            return;
        }
        size_t newSize = _size - size;
        std::memmove(begin.ptr(), end.ptr(), newSize);
        resize(newSize);
        _allocator[_size] = '\0';
    }

    constexpr StringView slice(size_t offset, size_t size = NoPos)
    {
        if (size == NoPos) {
            size = _size - offset;
        }
        return StringView(_allocator.ptr + offset, size);
    }

    constexpr StringView slice(Iterator begin, Iterator end)
    {
        return StringView(begin.ptr(), end.ptr() - begin.ptr());
    }

    constexpr const StringView slice(size_t offset, size_t size = NoPos) const
    {
        if (size == NoPos) {
            size = _size - offset;
        }
        return StringView(_allocator.ptr + offset, size);
    }

    constexpr const StringView slice(ConstIterator begin, ConstIterator end) const
    {
        return StringView(begin.ptr(), end.ptr() - begin.ptr());
    }

    constexpr BasicString copy_slice(size_t offset, size_t size = NoPos) const
    {
        return BasicString(slice(offset, size));
    }

    constexpr BasicString copy_slice(ConstIterator begin, ConstIterator end) const
    {
        return BasicString(slice(begin.ptr(), end.ptr() - begin.ptr()));
    }

    constexpr void write_to_buffer(char* buffer, size_t maxSize) const
    {
        std::strncpy(buffer, _allocator.ptr, blerg::clamp<size_t>(_size, 0, maxSize));
    }

    constexpr void to_upper()
    {
        for (size_t i = 0; i < _size; i++) {
            _allocator[i] = (char)toupper(_allocator[i]);
        }
    }

    constexpr void ToLower()
    {
        for (size_t i = 0; i < _size; i++) {
            _allocator[i] = (char)tolower(_allocator[i]);
        }
    }

    constexpr void to_camel_case()
    {
        bool first = true;
        for (size_t i = 0; i < _size; i++) {
            if (_allocator[i] == ' ' || _allocator[i] == '-' || _allocator[i] == '_') {
                Iterator offset = begin() + i;
                erase(offset, offset + 1);
                if (!first) {
                    _allocator[i] = (char)toupper(_allocator[i]);
                }
            }
            else {
                first         = false;
                _allocator[i] = (char)tolower(_allocator[i]);
            }
        }
    }

    constexpr void to_pascal_case()
    {
        bool first = true;
        for (size_t i = 0; i < _size; i++) {
            if (first) {
                _allocator[i] = (char)toupper(_allocator[i]);
                first         = false;
            }
            else if (_allocator[i] == ' ' || _allocator[i] == '-' || _allocator[i] == '_') {
                Iterator offset = begin() + i;
                erase(offset, offset + 1);
                _allocator[i] = (char)toupper(_allocator[i]);
            }
            else {
                _allocator[i] = (char)tolower(_allocator[i]);
            }
        }
    }

    constexpr void trim(char trim = ' ')
    {
        if (is_empty()) {
            return;
        }
        size_t prefix        = 0;
        size_t suffix        = 0;
        bool counting_prefix = true;
        for (size_t i = 0; i < _size; i++) {
            if (counting_prefix) {
                if (_allocator[i] != trim) {
                    counting_prefix = false;
                }
                else {
                    prefix++;
                }
            }
            else {
                if (_allocator[i] == trim) {
                    suffix++;
                }
                else {
                    suffix = 0;
                }
            }
        }
        pop_back(suffix);
        pop_front(prefix);
    }

    constexpr void trim_trailing(char trim = ' ')
    {
        size_t count = 0;
        for (size_t i = 0; i < _size; i++) {
            if (_allocator[i] == trim) {
                count++;
            }
            else {
                count = 0;
            }
        }
        pop_back(count);
    }

    constexpr size_t find(const BasicString& str) const
    {
        return find(StringView(str._allocator.ptr, str._size));
    }

    constexpr size_t find(const StringView& str) const
    {
        return StringView(_allocator.ptr, _size).find(str);
    }

    constexpr size_t Find(const char* str) const { return find(StringView(str)); }

    constexpr size_t find_last(const BasicString& str) const
    {
        return find_last(StringView(str._allocator.ptr, str._size));
    }

    constexpr size_t find_last(const StringView& str) const
    {
        return StringView(_allocator.ptr, _size).find_last(str);
    }

    constexpr size_t find_last(const char* str) const { return find_last(StringView(str)); }

    template <typename _ArrayAllocator>
    constexpr void FindAll(Array<size_t, _ArrayAllocator>& buffer, const BasicString& str) const
    {
        find_all(buffer, StringView(str._allocator.ptr, str._size));
    }

    template <typename _ArrayAllocator>
    constexpr void find_all(Array<size_t, _ArrayAllocator>& buffer, const StringView& str) const
    {
        StringView(_allocator.ptr, _size).find_all(buffer, str);
    }

    template <typename _ArrayAllocator>
    constexpr void find_all(Array<size_t, _ArrayAllocator>& buffer, const char* str) const
    {
        find_all(buffer, StringView(str));
    }

    constexpr void replace(const BasicString& search, const BasicString& replace)
    {
        this->replace(StringView(search._allocator.ptr, search._size),
                      StringView(replace._allocator.ptr, replace._size));
    }

    constexpr bool replace_first(const BasicString& search, const BasicString& replace)
    {
        return replace_first(StringView(search._allocator.ptr, search._size),
                             StringView(replace._allocator.ptr, replace._size));
    }

    constexpr bool replace_last(const BasicString& search, const BasicString& replace)
    {
        return replace_last(StringView(search._allocator.ptr, search._size),
                            StringView(replace._allocator.ptr, replace._size));
    }

    constexpr void replace(const StringView& search, const StringView& replace)
    {
        bool next = true;
        while (next) {
            next = replace_first(search, replace);
        }
    }

    constexpr bool replace_first(const StringView& search, const StringView& replace)
    {
        size_t offset = find(search);
        return _replace_instance(offset, search.size(), replace);
    }

    constexpr bool replace_last(const StringView& search, const StringView& replace)
    {
        size_t offset = find_last(search);
        return _replace_instance(offset, search.size(), replace);
    }

    constexpr void replace(const char* search, const char* replace)
    {
        this->replace(StringView(search), StringView(replace));
    }

    constexpr bool replace_first(const char* search, const char* replace)
    {

        return replace_first(StringView(search), StringView(replace));
    }

    constexpr bool replace_last(const char* search, const char* replace)
    {
        return replace_last(StringView(search), StringView(replace));
    }

private:
    size_t _size          = 0;
    Allocation _allocator = {};

    constexpr bool _replace_instance(size_t pos, size_t search_size, const StringView& replace)
    {
        if (pos == NoPos) {
            return false;
        }
        resize(_size - search_size + replace.size());

        Iterator offset = begin() + pos;
        std::memmove(offset.ptr() + replace.size(), offset.ptr() + search_size,
                     end().ptr() - (offset.ptr() + replace.size()));
        std::strncpy(offset.ptr(), replace.data(), replace.size());
        _allocator[_size] = '\0';
        return true;
    }
};

template <typename T, typename _Allocator, size_t _CapacityIncreaseIntervalSize>
struct StringViewCast<BasicString<T, _Allocator, _CapacityIncreaseIntervalSize>> {
    static constexpr StringView
        Cast(const BasicString<T, _Allocator, _CapacityIncreaseIntervalSize>& str)
    {
        return StringView(str.data(), str._size);
    }
};

typedef BasicString<char> String;

template <size_t TSize>
using StringBuffer = BasicString<char, FixedAllocation<TSize>>;

} // namespace blerg

#endif
