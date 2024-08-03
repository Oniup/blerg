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

#ifndef CORE_STRINGS__CSTR_UTIL_H
#define CORE_STRINGS__CSTR_UTIL_H

#include <cstdio>

namespace blerg {

// Calculates the length of a null-terminated C-string
//
// - cstr: The C-string to calculate the length of
// Returns: The length of the C-string. If the input is a null pointer, the function returns 0.
constexpr size_t cstr_length(const char* cstr)
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
constexpr bool cstr_compare(const char* cstr, const char* cstr2, size_t size)
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
constexpr void cstr_copy(char* dest, const char* src, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

constexpr bool is_upper_case(char32_t ch)
{
    return (ch >= 'A' && ch <= 'Z');
}

constexpr bool is_lower_case(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z');
}

constexpr bool is_digit(char32_t ch)
{
    return (ch >= '0' && ch <= '9');
}

constexpr bool is_hex_digit(char32_t ch)
{
    return (is_digit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
}

constexpr bool is_binary_digit(char32_t ch)
{
    return (ch == '0' || ch == '1');
}

constexpr bool is_alphabet_char(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

constexpr bool is_alpha_numeric_char(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

constexpr bool is_identifier(char32_t ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
           ch == '_';
}

constexpr bool IsSymbol(char32_t ch)
{
    return ch != '_' &&
           ((ch >= '!' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '`') ||
            (ch >= '{' && ch <= '~') || ch == '\t' || ch == ' ');
}

constexpr bool is_control(char32_t ch)
{
    return (ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
}

constexpr bool is_whitespace(char32_t ch)
{
    return (ch == ' ') || (ch == 0x00a0) || (ch == 0x1680) || (ch >= 0x2000 && ch <= 0x200a) ||
           (ch == 0x202f) || (ch == 0x205f) || (ch == 0x3000) || (ch == 0x2028) ||
           (ch == 0x2029) || (ch >= 0x0009 && ch <= 0x000d) || (ch == 0x0085);
}

constexpr bool is_linebreak(char32_t ch)
{
    return (ch >= 0x000a && ch <= 0x000d) || (ch == 0x0085) || (ch == 0x2028) || (ch == 0x2029);
}

constexpr bool is_punctuation(char32_t ch)
{
    return (ch >= ' ' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '^') ||
           (ch == '`') || (ch >= '{' && ch <= '~') || (ch >= 0x2000 && ch <= 0x206f) ||
           (ch >= 0x3000 && ch <= 0x303f);
}

constexpr bool is_underscore(char32_t ch)
{
    return (ch == '_');
}

} // namespace blerg

#endif
