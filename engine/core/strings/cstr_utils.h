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

#ifndef IGNITE_CORE_STRINGS__CSTR_UTIL_H
#define IGNITE_CORE_STRINGS__CSTR_UTIL_H

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

static constexpr bool IsUpperCase(char32_t ch)
{
    return (ch >= 'A' && ch <= 'Z');
}

static constexpr bool IsLowerCase(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z');
}

static constexpr bool IsDigit(char32_t ch)
{
    return (ch >= '0' && ch <= '9');
}

static constexpr bool IsHexDigit(char32_t ch)
{
    return (IsDigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
}

static constexpr bool IsBinaryDigit(char32_t ch)
{
    return (ch == '0' || ch == '1');
}

static constexpr bool IsAlphabetChar(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static constexpr bool IsAlphaNumericChar(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

static constexpr bool IsIdentifierChar(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
           ch == '_';
}

static constexpr bool IsSymbol(char32_t ch)
{
    return ch != '_' &&
           ((ch >= '!' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '`') ||
            (ch >= '{' && ch <= '~') || ch == '\t' || ch == ' ');
}

static constexpr bool IsControl(char32_t ch)
{
    return (ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
}

static constexpr bool IsWhitespace(char32_t ch)
{
    return (ch == ' ') || (ch == 0x00a0) || (ch == 0x1680) || (ch >= 0x2000 && ch <= 0x200a) ||
           (ch == 0x202f) || (ch == 0x205f) || (ch == 0x3000) || (ch == 0x2028) ||
           (ch == 0x2029) || (ch >= 0x0009 && ch <= 0x000d) || (ch == 0x0085);
}

static constexpr bool IsLinebreak(char32_t ch)
{
    return (ch >= 0x000a && ch <= 0x000d) || (ch == 0x0085) || (ch == 0x2028) || (ch == 0x2029);
}

static constexpr bool IsPunct(char32_t ch)
{
    return (ch >= ' ' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '^') ||
           (ch == '`') || (ch >= '{' && ch <= '~') || (ch >= 0x2000 && ch <= 0x206f) ||
           (ch >= 0x3000 && ch <= 0x303f);
}

static constexpr bool IsUnderscore(char32_t ch)
{
    return (ch == '_');
}

} // namespace Ignite

#endif
