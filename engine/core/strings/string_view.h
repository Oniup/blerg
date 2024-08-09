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

#ifndef CORE_STRINGS__STRING_VIEW_H
#define CORE_STRINGS__STRING_VIEW_H

#include "core/containers/array.h"
#include "core/containers/iterator.h"
#include "core/limits.h"
#include "core/strings/cstr_utils.h"

namespace fiwre {

template <typename _T>
struct StringViewCast {};

struct StringView {
  using ConstIterator           = ConstPackedIterator<char, StringView>;
  static constexpr size_t NoPos = NumericLimits<size_t>::max();

  constexpr StringView() = default;
  constexpr StringView(const char* cstr, size_t size)
    : _size(size), _ptr(cstr) {}

  constexpr StringView(const char* cstr)
    : _size(fiwre::cstr_length(cstr)), _ptr(cstr) {}

  template <typename _StrT>
  constexpr StringView(const _StrT& str)
    : StringView(StringViewCast<_StrT>::cast(str)) {}

  constexpr StringView(const StringView& view)
    : _size(view._size), _ptr(view._ptr) {}

  constexpr StringView(StringView&& view)
    : _size(view._size), _ptr(view._ptr) {
    view._size = 0;
    view._ptr  = nullptr;
  }

  constexpr ConstIterator begin() const {
    return ConstIterator(const_cast<StringView*>(this), 0);
  }

  constexpr ConstIterator end() const {
    return ConstIterator(const_cast<StringView*>(this), _size);
  }

  constexpr const char* data() const { return _ptr; }
  constexpr size_t size() const { return _size; }

  constexpr const char& at(size_t index) const { return _ptr[index]; }
  constexpr const char& first() const { return _ptr[0]; }
  constexpr const char& last() const { return _ptr[_size - 1]; }

  inline constexpr bool is_empty() const { return _ptr == nullptr; }
  inline constexpr bool is_null_terminated() const {
    return _size == 0 || _ptr[_size] == '\0';
  }

  constexpr char operator[](size_t offset) const { return _ptr[offset]; }
  constexpr bool operator==(const StringView& view) const {
    return compare(view);
  }
  constexpr bool operator!=(const StringView& view) const {
    return !compare(view);
  }

  template <typename _StrT>
  inline constexpr bool operator==(const _StrT& str) const {
    return StringViewCast<_StrT>::Cast(str) == *this;
  }

  template <typename _StrT>
  inline constexpr bool operator!=(const _StrT& str) const {
    return StringViewCast<_StrT>::Cast(str) != *this;
  }

  constexpr StringView& operator=(const char* str) {
    _ptr  = str;
    _size = fiwre::cstr_length(str);
    return *this;
  }

  constexpr StringView& operator=(const StringView& str) {
    _ptr  = str._ptr;
    _size = str._size;
    return *this;
  }

  constexpr StringView& operator=(StringView&& str) {
    _ptr  = str._ptr;
    _size = str._size;

    str._ptr  = nullptr;
    str._size = 0;
    return *this;
  }

  template <typename _StrT>
  inline constexpr StringView& operator=(const _StrT& str) {
    *this = StringViewCast<_StrT>::Cast(str);
    return *this;
  }

  constexpr StringView slice(size_t offset, size_t size = NoPos) const {
    if (size == NoPos) {
      size = _size - offset;
    }
    return StringView(_ptr + offset, size);
  }

  constexpr void set(const StringView& str) {
    _ptr  = str._ptr;
    _size = str._size;
  }

  constexpr bool compare(const StringView& str) const {
    if (str._size != _size) {
      return false;
    }
    return fiwre::cstr_compare(_ptr, str._ptr, _size);
  }

  constexpr size_t find(const StringView& match) const {
    for (size_t i = 0; i < _size; i++) {
      if (match._size > _size - i) {
        return NoPos;
      } else if (slice(i, match._size) == match) {
        return i;
      }
    }
    return NoPos;
  }

  constexpr size_t find_last(const StringView& match) const {
    for (size_t i = _size - match._size; i >= 0; i--) {
      if (match._size > _size - i) {
        return NoPos;
      } else if (slice(i, match._size) == match) {
        return i;
      }
    }
    return NoPos;
  }

  template <typename _ArrayAllocator>
  constexpr void find_all(Array<size_t, _ArrayAllocator>& buffer,
                          const StringView& match) const {
    if (!(match._size > 0)) {
      return;
    }
    for (size_t i = 0; i < _size; i++) {
      if (match._size > _size - i) {
        break;
      } else if (slice(i, match._size) == match) {
        buffer.push_back(i);
      }
    }
  }

private:
  size_t _size     = 0;
  const char* _ptr = nullptr;
};

template <>
struct StringViewCast<StringView> {
  static constexpr StringView cast(const StringView& view) { return view; }
};

template <>
struct StringViewCast<const char*> {
  static constexpr StringView cast(const char* cstr) {
    return StringView(cstr, fiwre::cstr_length(cstr));
  }
};

template <>
struct StringViewCast<char*> {
  static constexpr StringView cast(char* cstr) {
    return StringView(cstr, fiwre::cstr_length(cstr));
  }
};

template <size_t _Size>
struct StringViewCast<char[_Size]> {
  static constexpr StringView cast(char (&cstr)[_Size]) {
    return StringView(cstr, fiwre::cstr_length(cstr));
  }
};
template <size_t _Size>
struct StringViewCast<const char[_Size]> {
  static constexpr StringView cast(const char (&cstr)[_Size]) {
    return StringView(cstr, fiwre::cstr_length(cstr));
  }
};

} // namespace fiwre

#endif
