// This file is part of Kryos Engine (https://github.com/Oniup/kryos-engine)
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

#ifndef IGNITE_CORE__CSTR_UTIL_H
#define IGNITE_CORE__CSTR_UTIL_H

#include <cstdio>

namespace Ignite {

// Calculates the length of a null-terminated C-string
//
// - cstr: The C-string to calculate the length of
// Returns: The length of the C-string. If the input is a null pointer, the function returns 0.
constexpr size_t CStrLength(const char* cstr)
{
    if (cstr == nullptr) {
        return 0;
    }
    size_t size = 0;
    while (cstr[size] != '\0') {
        size++;
    }
    return size;
}

// Compares two C-strings for equality up to a specified size
//
// - cstr: The first C-string to compare
// - cstr2: The second C-string to compare
// - size: The maximum number of characters to compare
// Returns: True if the C-strings are equal up to the specified size, false otherwise.
constexpr bool CStrCompare(const char* cstr, const char* cstr2, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if (cstr[i] != cstr2[i]) {
            return false;
        }
    }
    return true;
}

// Assigns the characters from one C-string to another up to a specified size
//
// - dest: The destination C-string
// - src: The source C-string
// - size: The number of characters to assign
// WARNING: The function does not null-terminate the destination string.
constexpr void CStrCopy(char* dest, const char* src, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

} // namespace Ignite

#endif
