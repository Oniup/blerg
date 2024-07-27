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

#ifndef IGNITE_CORE_MEMORY__UTILS_H
#define IGNITE_CORE_MEMORY__UTILS_H

#include "core/defines.h"
#include "core/type_traits.h"
#include <initializer_list>
#include <new>
#include <type_traits>

namespace Ignite {

template <typename T>
IG_FORCE_INLINE constexpr unsigned char* ByteCast(T& val)
{
    union {
        T* val;
        unsigned char* byte_chars;
    } pun {&val};
    return pun.byte_chars;
}

template <typename TTo, typename TFrom>
IG_FORCE_INLINE constexpr TTo* ConstexprPtrCast(TFrom* val)
{
    union {
        TFrom* from;
        TTo* result;
    } pun {val};
    return pun.result;
}

template <typename T>
IG_FORCE_INLINE constexpr typename std::remove_reference<T>::type&& Move(T&& lval)
{
    return static_cast<typename std::remove_reference<T>::type&&>(lval);
}

IG_FORCE_INLINE constexpr void* CopyBytes(void* dest, const void* src, size_t size)
{
    unsigned char* dest_bytes      = static_cast<unsigned char*>(dest);
    const unsigned char* src_bytes = static_cast<const unsigned char*>(src);
    for (size_t i = 0; i < size; i++) {
        dest_bytes[i] = src_bytes[i];
    }
    return dest_bytes;
}

template <typename Iter>
IG_FORCE_INLINE constexpr void DefaultConstructItems(Iter begin, Iter end)
{
    if constexpr (std::is_default_constructible_v<typename Iter::Type>) {
        for (Iter iter = begin; iter != end; iter++) {
            new (iter.Ptr()) typename Iter::Type();
        }
    }
}

template <typename Iter, typename... _Args>
IG_FORCE_INLINE constexpr void FillConstructItems(Iter begin, Iter end, const _Args&... args)
{
    for (Iter iter = begin; iter != end; iter++) {
        new (iter.Ptr()) typename Iter::Type(args...);
    }
}

template <typename Iter>
IG_FORCE_INLINE constexpr void Fill(Iter begin, Iter end, const typename Iter::Type& fill)
{
    for (Iter iter = begin; iter != end; iter++) {
        *iter = fill;
    }
}

// Warning: If items are already initialized, make sure use
template <typename Iter>
IG_FORCE_INLINE constexpr void
    CopyInitializerListItems(Iter begin, const std::initializer_list<typename Iter::Type>& list)
{
    using Type = typename Iter::Type;
    Iter iter  = begin;
    for (const Type& val : list) {
        if constexpr (std::is_move_constructible_v<Type>) {
            new (iter.Ptr()) Type(val);
        }
        else {
            *iter.Ptr() = val;
        }
        iter++;
    }
}

template <typename T, typename... _Args, size_t... Indices>
IG_FORCE_INLINE constexpr void
    ConstructArgsIndices(T& dest, TypeTraits::index_sequence<Indices...>, _Args&&... args)
{
    if constexpr (std::is_pointer<T>::value) {
        new (dest) typename std::remove_pointer<T>::type {(void(Indices), args)...};
    }
    else {
        new (&dest) T {(void(Indices), args)...};
    }
}

template <typename T>
IG_FORCE_INLINE constexpr void DefaultConstruct(T& obj)
{
    if constexpr (std::is_pointer<T>::value) {
        new (obj) typename std::remove_pointer<T>::type();
    }
    else {
        new (&obj) T();
    }
}

template <typename T>
IG_FORCE_INLINE constexpr void CopyConstruct(T& dest, const T& src)
{
    if constexpr (std::is_pointer<T>::value) {
        new (dest) typename std::remove_pointer<T>::type(src);
    }
    else {
        new (&dest) T(src);
    }
}

template <typename T>
IG_FORCE_INLINE constexpr void MoveConstruct(T& dest, T&& src)
{
    if constexpr (std::is_pointer<T>::value) {
        new (dest) typename std::remove_pointer<T>::type(Ignite::Move(src));
    }
    else {
        new (&dest) T(Ignite::Move(src));
    }
}

template <typename Iter>
IG_FORCE_INLINE constexpr void CopyItems(Iter begin, Iter end, const typename Iter::Type* src)
{
    using Type  = typename Iter::Type;
    size_t size = end.Ptr() - begin.Ptr();
    Iter iter   = begin;
    for (size_t i = 0; i < size; i++, iter++) {
        if constexpr (std::is_copy_constructible_v<Type>) {
            new (iter.Ptr()) Type(src[i]);
        }
        else {
            *iter.Ptr() = src[i];
        }
    }
}

template <typename Iter>
IG_FORCE_INLINE constexpr void MoveItems(Iter begin, Iter end, const typename Iter::Type* src)
{
    using Type  = typename Iter::Type;
    size_t size = end.Ptr() - begin.Ptr();
    Iter iter   = begin;
    for (size_t i = 0; i < size; i++, iter++) {
        if constexpr (std::is_move_constructible_v<Type>) {
            new (iter.Ptr()) Type(Ignite::Move(src[i]));
        }
        else {
            *iter.Ptr() = Ignite::Move(src[i]);
        }
    }
}

template <typename Iter>
IG_FORCE_INLINE constexpr void ShiftItems(Iter begin, Iter end, Iter pos)
{
    using Type  = typename Iter::Type;
    size_t size = end.Ptr() - begin.Ptr();
    if (pos.Ptr() < begin.Ptr()) {
        for (size_t i = 0; i < size; i++) {
            new ((pos + i).Ptr()) Type(Ignite::Move(*(begin + i)));
        }
    }
    else {
        for (int i = size - 1; i >= 0; --i) {
            new ((pos + i).Ptr()) Type(Ignite::Move(*(begin + i)));
        }
    }
}

template <typename Iter>
IG_FORCE_INLINE constexpr void DestructItems(Iter begin, Iter end)
{
    using T = typename Iter::Type;
    if constexpr (std::is_destructible<T>::value) {
        for (Iter iter = begin; iter != end; iter++) {
            iter->~T();
        }
    }
}

template <typename T>
IG_FORCE_INLINE constexpr void Destruct(T& obj)
{
    if constexpr (std::is_pointer<T>::value) {
        using Type = typename std::remove_pointer<T>::type;
        obj->~Type();
    }
    else {
        obj.~T();
    }
}

template <typename T>
IG_FORCE_INLINE constexpr void Swap(T&& obj1, T&& obj2)
{
    T tmp = Ignite::Move(obj1);
    obj1  = Ignite::Move(obj2);
    obj2  = Ignite::Move(tmp);
}

} // namespace Ignite

#endif
