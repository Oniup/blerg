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

#ifndef IGNITE_CORE__TYPE_TRAITS_H
#define IGNITE_CORE__TYPE_TRAITS_H

#include <cstdio>
#include <type_traits>

namespace Ignite::TypeTraits {

template <size_t... _Indices>
struct index_sequence {
    using type = index_sequence<_Indices...>;
    static constexpr size_t size() { return sizeof...(_Indices); }
};

namespace Impl {

template <size_t _N, size_t... _Indices>
struct make_index_sequence : make_index_sequence<_N - 1, _N - 1, _Indices...> {};

template <size_t... _Indices>
struct make_index_sequence<0, _Indices...> : index_sequence<_Indices...> {};

} // namespace Impl

template <size_t _N>
using make_index_sequence = typename Impl::make_index_sequence<_N>::type;

template <size_t _Index, typename _Head, typename... _Rest>
struct tuple_type {
    using type = typename tuple_type<_Index - 1, _Rest...>::type;
};

template <typename _Head, typename... _Rest>
struct tuple_type<0, _Head, _Rest...> {
    using type = _Head;
};

template <typename _T>
struct remove_all_pointers {
    using type = _T;
};

template <typename _T>
struct remove_all_pointers<_T*> {
    using type = typename remove_all_pointers<_T>::type;
};

template <typename _T>
struct pointer_count {
    static constexpr size_t value =
        std::is_pointer<_T>::value
            ? 1 + pointer_count<typename std::remove_pointer<_T>::type>::value
            : 0;
};

} // namespace Ignite::TypeTraits

#endif
