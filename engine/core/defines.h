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

#ifndef IGNITE_CORE__DEFINES_H
#define IGNITE_CORE__DEFINES_H

#if defined(__clang__) || defined(__GNUG__) && defined(__GNUC__)
#    define IG_GENERATE_TRAP() __builtin_trap()
#    define IG_THREADLOCAL __thread
#    define IG_STDCALL __attribute__((stdcall))
#    define IG_CDECL __attribute__((cdecl))
#    define IG_RESTRICT __restrict__
#    define IG_FORCE_INLINE __attribute__((always_inline)) inline
#    define IG_FORCE_NOINLINE __attribute__((noinline))
#    define IG_NO_RETURN __attribute__((noreturn))
#    define IG_DEPRECATED [[deprecated]]
#elif defined(_MSC_VER)
#    if _MSC_VER < 1900
#        error "Required Visual Studio 2015 or newer."
#    endif
#    define IG_GENERATE_TRAP() __debugbreak()
#    define IG_THREADLOCAL __declspec(thread)
#    define IG_STDCALL __stdcall
#    define IG_CDECL __cdecl
#    define IG_RESTRICT __restrict
#    define IG_FORCE_INLINE __forceinline
#    define IG_FORCE_NOINLINE __declspec(noinline)
#    define IG_NO_RETURN __declspec(noreturn)
#    define IG_DEPRECATED __declspec(deprecated)

#    undef __PRETTY_FUNCTION__
#    define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#    error "Unknown compiler"
#endif

#ifndef NDEBUG
#    define IG_BASIC_ASSERT(__condition, msg)                                                     \
        if ((__condition)) {                                                                      \
            std::fprintf(stderr, "expr=(%s): '%s' => file=%s, func=%s:%d\n", #__condition, (msg), \
                         __FILE__, __FUNCTION__, __LINE__);                                       \
            IG_GENERATE_TRAP();                                                                   \
        }                                                                                         \
        else                                                                                      \
            ((void)0)
#else
#    define IG_BASIC_ASSERT(__condition, msg)
#endif

#ifndef IG_CUSTOM_ALLOCATE_HEAP_MEMORY
#    define IG_ALLOCATE_HEAP_MEMORY(__size) std::malloc(__size)
#endif
#ifndef IG_CUSTOM_FREE_HEAP_MEMORY
#    define IG_FREE_HEAP_MEMORY(__ptr) std::free(__ptr)
#endif
#ifndef IG_CUSTOM_REALLOCATE_HEAP_MEMORY
#    define IG_REALLOCATE_HEAP_MEMORY(__ptr, __new_size) std::realloc(__ptr, __new_size)
#endif

#define IG_DEFAULT_CAPACITY_INTERVAL 20

#endif
