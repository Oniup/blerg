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

template <typename _T>
IG_FORCE_INLINE constexpr unsigned char* ByteCast(_T& val)
{
    union {
        _T* val;
        unsigned char* byte_chars;
    } pun {&val};
    return pun.byte_chars;
}

template <typename _To, typename _From>
IG_FORCE_INLINE constexpr _To* ConstexprPtrCast(_From* val)
{
    union {
        _From* from;
        _To* result;
    } pun {val};
    return pun.result;
}

template <typename _T>
IG_FORCE_INLINE constexpr typename std::remove_reference<_T>::type&& Move(_T&& lval)
{
    return static_cast<typename std::remove_reference<_T>::type&&>(lval);
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

template <typename _Iter>
IG_FORCE_INLINE constexpr void DefaultConstructItems(_Iter begin, _Iter end)
{
    if constexpr (std::is_default_constructible_v<typename _Iter::Type>) {
        for (_Iter iter = begin; iter != end; iter++) {
            new (iter.Ptr()) typename _Iter::Type();
        }
    }
}

template <typename _Iter, typename... _Args>
IG_FORCE_INLINE constexpr void FillConstructItems(_Iter begin, _Iter end, const _Args&... args)
{
    for (_Iter iter = begin; iter != end; iter++) {
        new (iter.Ptr()) typename _Iter::Type(args...);
    }
}

template <typename _Iter>
IG_FORCE_INLINE constexpr void Fill(_Iter begin, _Iter end, const typename _Iter::Type& fill)
{
    for (_Iter iter = begin; iter != end; iter++) {
        *iter = fill;
    }
}

// Warning: If items are already initialized, make sure use
template <typename _Iter>
IG_FORCE_INLINE constexpr void
    CopyInitializerListItems(_Iter begin, const std::initializer_list<typename _Iter::Type>& list)
{
    using Type = typename _Iter::Type;
    _Iter iter = begin;
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

template <typename T, typename... _Args, size_t... _Indices>
IG_FORCE_INLINE constexpr void
    ConstructArgsIndices(T& dest, TypeTraits::index_sequence<_Indices...>, _Args&&... args)
{
    if constexpr (std::is_pointer<T>::value) {
        new (dest) typename std::remove_pointer<T>::type {(void(_Indices), args)...};
    }
    else {
        new (&dest) T {(void(_Indices), args)...};
    }
}

template <typename _T>
IG_FORCE_INLINE constexpr void DefaultConstruct(_T& obj)
{
    if constexpr (std::is_pointer<_T>::value) {
        new (obj) typename std::remove_pointer<_T>::type();
    }
    else {
        new (&obj) _T();
    }
}

template <typename _T>
IG_FORCE_INLINE constexpr void CopyConstruct(_T& dest, const _T& src)
{
    if constexpr (std::is_pointer<_T>::value) {
        new (dest) typename std::remove_pointer<_T>::type(src);
    }
    else {
        new (&dest) _T(src);
    }
}

template <typename _T>
IG_FORCE_INLINE constexpr void MoveConstruct(_T& dest, _T&& src)
{
    if constexpr (std::is_pointer<_T>::value) {
        new (dest) typename std::remove_pointer<_T>::type(Ignite::Move(src));
    }
    else {
        new (&dest) _T(Ignite::Move(src));
    }
}

template <typename _Iter>
IG_FORCE_INLINE constexpr void CopyItems(_Iter begin, _Iter end, const typename _Iter::Type* src)
{
    using Type  = typename _Iter::Type;
    size_t size = end.Ptr() - begin.Ptr();
    _Iter iter  = begin;
    for (size_t i = 0; i < size; i++, iter++) {
        if constexpr (std::is_copy_constructible_v<Type>) {
            new (iter.Ptr()) Type(src[i]);
        }
        else {
            *iter.Ptr() = src[i];
        }
    }
}

template <typename _Iter>
IG_FORCE_INLINE constexpr void MoveItems(_Iter begin, _Iter end, const typename _Iter::Type* src)
{
    using Type  = typename _Iter::Type;
    size_t size = end.Ptr() - begin.Ptr();
    _Iter iter  = begin;
    for (size_t i = 0; i < size; i++, iter++) {
        if constexpr (std::is_move_constructible_v<Type>) {
            new (iter.Ptr()) Type(Ignite::Move(src[i]));
        }
        else {
            *iter.Ptr() = Ignite::Move(src[i]);
        }
    }
}

template <typename _Iter>
IG_FORCE_INLINE constexpr void ShiftItems(_Iter begin, _Iter end, _Iter pos)
{
    using Type  = typename _Iter::Type;
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

template <typename _Iter>
IG_FORCE_INLINE constexpr void DestructItems(_Iter begin, _Iter end)
{
    using T = typename _Iter::Type;
    if constexpr (std::is_destructible<T>::value) {
        for (_Iter iter = begin; iter != end; iter++) {
            iter->~T();
        }
    }
}

template <typename _T>
IG_FORCE_INLINE constexpr void Destruct(_T& obj)
{
    if constexpr (std::is_pointer<_T>::value) {
        using T = typename std::remove_pointer<_T>::type;
        obj->~T();
    }
    else {
        obj.~_T();
    }
}

template <typename _T>
IG_FORCE_INLINE constexpr void Swap(_T&& obj1, _T&& obj2)
{
    _T tmp = Ignite::Move(obj1);
    obj1   = Ignite::Move(obj2);
    obj2   = Ignite::Move(tmp);
}

} // namespace Ignite

#endif
