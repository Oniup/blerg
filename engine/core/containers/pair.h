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

#ifndef CORE_CONTAINERS__PAIR_H
#define CORE_CONTAINERS__PAIR_H

#include "core/memory/utils.h"

namespace blerg {

template <typename TKey, typename TValue>
struct IgPair {
    using KeyType   = TKey;
    using ValueType = TValue;

    TKey key     = {};
    TValue value = {};

    constexpr IgPair() = default;

    constexpr IgPair(const TKey& first)
          : key(first), value()
    {
    }

    constexpr IgPair(const TKey& first, const TValue& second)
          : key(first), value(second)
    {
    }

    constexpr IgPair(const TKey& first, TValue&& second)
          : key(first), value(blerg::move(second))
    {
    }

    constexpr IgPair(TKey&& first, TValue&& second)
          : key(blerg::move(first)), value(blerg::move(second))
    {
    }

    ~IgPair() = default;

    constexpr bool operator==(const IgPair<TKey, TValue>& pair) const
    {
        return (key == pair.key) && (value == pair.value);
    }

    constexpr bool operator!=(const IgPair<TKey, TValue>& pair) const
    {
        return (key != pair.key) || (value != pair.value);
    }

    constexpr bool operator>=(const IgPair<TKey, TValue>& pair)
    {
        if (key != pair.key) {
            return key < pair.key;
        }
        return value < pair.value;
    }
};

} // namespace blerg

#endif
