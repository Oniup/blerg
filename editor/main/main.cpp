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

#include "core/types/array.h"
#include <cstdio>

int main()
{
    IgArray<const char*> message      = {"Dynamic", "array"};
    const IgArray<const char*> extend = {
        "however,",
        "this",
        "is",
        "copied",
    };

    IgArray message_copy(message);
    message_copy.PushBack(extend);

    std::printf("From message: ");
    for (const char* text : message) {
        std::printf("%s ", text);
    }
    std::printf("\nFrom Copyed: ");
    for (const char* text : message_copy) {
        std::printf("%s ", text);
    }
    std::printf("\n");
}
