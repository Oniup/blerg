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

#ifndef CORE_MEMORY__UTILS_H
#define CORE_MEMORY__UTILS_H

#include "core/defines.h"
#include "core/type_traits.h"
#include <initializer_list>
#include <new>
#include <type_traits>

namespace fiwre {

template <typename T>
FORCE_INLINE constexpr unsigned char* byte_cast(T& val) {
  union {
    T* val;
    unsigned char* byte_chars;
  } pun {&val};
  return pun.byte_chars;
}

template <typename TTo, typename TFrom>
FORCE_INLINE constexpr TTo* constexpr_ptr_cast(TFrom* val) {
  union {
    TFrom* from;
    TTo* result;
  } pun {val};
  return pun.result;
}

template <typename T>
FORCE_INLINE constexpr typename std::remove_reference<T>::type&&
    move(T&& lval) {
  return static_cast<typename std::remove_reference<T>::type&&>(lval);
}

FORCE_INLINE constexpr void* copy_bytes(void* dest, const void* src,
                                        size_t size) {
  unsigned char* dest_bytes      = static_cast<unsigned char*>(dest);
  const unsigned char* src_bytes = static_cast<const unsigned char*>(src);
  for (size_t i = 0; i < size; i++) {
    dest_bytes[i] = src_bytes[i];
  }
  return dest_bytes;
}

template <typename Iter>
FORCE_INLINE constexpr void default_construct_items(Iter begin, Iter end) {
  if constexpr (std::is_default_constructible_v<typename Iter::Type>) {
    for (Iter iter = begin; iter != end; iter++) {
      new (iter.ptr()) typename Iter::Type();
    }
  }
}

template <typename Iter, typename... _Args>
FORCE_INLINE constexpr void fill_construct_items(Iter begin, Iter end,
                                                 const _Args&... args) {
  for (Iter iter = begin; iter != end; iter++) {
    new (iter.ptr()) typename Iter::Type(args...);
  }
}

template <typename Iter>
FORCE_INLINE constexpr void fill(Iter begin, Iter end,
                                 const typename Iter::Type& fill) {
  for (Iter iter = begin; iter != end; iter++) {
    *iter = fill;
  }
}

// Warning: If items are already initialized, make sure use
template <typename Iter>
FORCE_INLINE constexpr void copy_initializer_list_items(
    Iter begin, const std::initializer_list<typename Iter::Type>& list) {
  using Type = typename Iter::Type;
  Iter iter  = begin;
  for (const Type& val : list) {
    if constexpr (std::is_move_constructible_v<Type>) {
      new (iter.ptr()) Type(val);
    } else {
      *iter.ptr() = val;
    }
    iter++;
  }
}

template <typename T, typename... _Args, size_t... Indices>
FORCE_INLINE constexpr void
    construct_args_indices(T& dest, type_traits::index_sequence<Indices...>,
                           _Args&&... args) {
  if constexpr (std::is_pointer<T>::value) {
    new (dest) typename std::remove_pointer<T>::type {(void(Indices), args)...};
  } else {
    new (&dest) T {(void(Indices), args)...};
  }
}

template <typename T>
FORCE_INLINE constexpr void default_construct(T& obj) {
  if constexpr (std::is_pointer<T>::value) {
    new (obj) typename std::remove_pointer<T>::type();
  } else {
    new (&obj) T();
  }
}

template <typename T>
FORCE_INLINE constexpr void copy_construct(T& dest, const T& src) {
  if constexpr (std::is_pointer<T>::value) {
    new (dest) typename std::remove_pointer<T>::type(src);
  } else {
    new (&dest) T(src);
  }
}

template <typename T>
FORCE_INLINE constexpr void move_construct(T& dest, T&& src) {
  if constexpr (std::is_pointer<T>::value) {
    new (dest) typename std::remove_pointer<T>::type(fiwre::move(src));
  } else {
    new (&dest) T(fiwre::move(src));
  }
}

template <typename Iter>
FORCE_INLINE constexpr void copy_items(Iter begin, Iter end,
                                       const typename Iter::Type* src) {
  using Type  = typename Iter::Type;
  size_t size = end.ptr() - begin.ptr();
  Iter iter   = begin;
  for (size_t i = 0; i < size; i++, iter++) {
    if constexpr (std::is_copy_constructible_v<Type>) {
      new (iter.ptr()) Type(src[i]);
    } else {
      *iter.ptr() = src[i];
    }
  }
}

template <typename Iter>
FORCE_INLINE constexpr void move_items(Iter begin, Iter end,
                                       const typename Iter::Type* src) {
  using Type  = typename Iter::Type;
  size_t size = end.ptr() - begin.ptr();
  Iter iter   = begin;
  for (size_t i = 0; i < size; i++, iter++) {
    if constexpr (std::is_move_constructible_v<Type>) {
      new (iter.ptr()) Type(fiwre::move(src[i]));
    } else {
      *iter.ptr() = fiwre::move(src[i]);
    }
  }
}

template <typename Iter>
FORCE_INLINE constexpr void shift_items(Iter begin, Iter end, Iter pos) {
  using Type  = typename Iter::Type;
  size_t size = end.ptr() - begin.ptr();
  if (pos.ptr() < begin.ptr()) {
    for (size_t i = 0; i < size; i++) {
      new ((pos + i).ptr()) Type(fiwre::move(*(begin + i)));
    }
  } else {
    for (int i = size - 1; i >= 0; --i) {
      new ((pos + i).ptr()) Type(fiwre::move(*(begin + i)));
    }
  }
}

template <typename Iter>
FORCE_INLINE constexpr void destruct_items(Iter begin, Iter end) {
  using T = typename Iter::Type;
  if constexpr (std::is_destructible<T>::value) {
    for (Iter iter = begin; iter != end; iter++) {
      iter->~T();
    }
  }
}

template <typename T>
FORCE_INLINE constexpr void destruct(T& obj) {
  if constexpr (std::is_pointer<T>::value) {
    using Type = typename std::remove_pointer<T>::type;
    obj->~Type();
  } else {
    obj.~T();
  }
}

template <typename T>
FORCE_INLINE constexpr void swap(T&& obj1, T&& obj2) {
  T tmp = fiwre::move(obj1);
  obj1  = fiwre::move(obj2);
  obj2  = fiwre::move(tmp);
}

} // namespace fiwre

#endif
