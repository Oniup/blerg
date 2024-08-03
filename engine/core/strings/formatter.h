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

#ifndef CORE_STRINGS__FORMATTER_H
#define CORE_STRINGS__FORMATTER_H

#include <stdint.h>
#include <stdio.h>

namespace blerg {

template <typename... TArgs>
struct FormatterArguments {};

template <typename T>
struct Formatter {
    using type = T;

    template <typename... TArgs>
    static constexpr void format(char* buffer, size_t size);
};

}

#endif
