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

#ifndef CORE__DEFINES_H
#define CORE__DEFINES_H

#if defined(__clang__) || defined(__GNUG__) && defined(__GNUC__)
#  define GENERATE_TRAP() __builtin_trap()
#  define THREADLOCAL __thread
#  define STDCALL __attribute__((stdcall))
#  define CDECL __attribute__((cdecl))
#  define RESTRICT __restrict__
#  define FORCE_INLINE __attribute__((always_inline)) inline
#  define FORCE_NOINLINE __attribute__((noinline))
#  define NO_RETURN __attribute__((noreturn))
#  define DEPRECATED [[deprecated]]
#elif defined(_MSC_VER)
#  if _MSC_VER < 1900
#    error "Required Visual Studio 2015 or newer."
#  endif
#  define GENERATE_TRAP() __debugbreak()
#  define THREADLOCAL __declspec(thread)
#  define STDCALL __stdcall
#  define CDECL __cdecl
#  define RESTRICT __restrict
#  define FORCE_INLINE __forceinline
#  define FORCE_NOINLINE __declspec(noinline)
#  define NO_RETURN __declspec(noreturn)
#  define DEPRECATED __declspec(deprecated)

#  undef __PRETTY_FUNCTION__
#  define __PRETTY_FUNCTION__ __FUNCS_
#else
#  error "Unknown compiler"
#endif

#ifndef NDEBUG
#  define BASIC_ASSERT(__condition, msg)                                   \
    if ((__condition)) {                                                   \
      std::fprintf(stderr, "expr=(%s): '%s' => file=%s, func=%s:%d\n",     \
                   #__condition, (msg), __FILE__, __FUNCTION__, __LINE__); \
      GENERATE_TRAP();                                                     \
    } else                                                                 \
      ((void)0)
#else
#  define BASIC_ASSERT(__condition, msg)
#endif

#ifndef CUSTOM_ALLOCATE_HEAP_MEMORY
#  define ALLOCATE_HEAP_MEMORY(__size) std::malloc(__size)
#endif
#ifndef CUSTOM_FREE_HEAP_MEMORY
#  define FREE_HEAP_MEMORY(__ptr) std::free(__ptr)
#endif
#ifndef CUSTOM_REALLOCATE_HEAP_MEMORY
#  define REALLOCATE_HEAP_MEMORY(__ptr, __new_size) \
    std::realloc(__ptr, __new_size)
#endif

#define DEFAULT_CAPACITY_INTERVAL 20

#endif
