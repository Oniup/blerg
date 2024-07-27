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

#ifndef IGNITE_CORE_CONTAINERS__PAIR_H
#define IGNITE_CORE_CONTAINERS__PAIR_H

#include "core/memory/utils.h"

template <typename TKey, typename TValue>
struct IgPair {
    using KeyType   = TKey;
    using ValueType = TValue;

    TKey Key     = {};
    TValue Value = {};

    constexpr IgPair() = default;

    constexpr IgPair(const TKey& first)
          : Key(first), Value()
    {
    }

    constexpr IgPair(const TKey& first, const TValue& second)
          : Key(first), Value(second)
    {
    }

    constexpr IgPair(const TKey& first, TValue&& second)
          : Key(first), Value(Ignite::Move(second))
    {
    }

    constexpr IgPair(TKey&& first, TValue&& second)
          : Key(Ignite::Move(first)), Value(Ignite::Move(second))
    {
    }

    ~IgPair() = default;

    constexpr bool operator==(const IgPair<TKey, TValue>& pair) const
    {
        return (Key == pair.Key) && (Value == pair.Value);
    }

    constexpr bool operator!=(const IgPair<TKey, TValue>& pair) const
    {
        return (Key != pair.Key) || (Value != pair.Value);
    }

    constexpr bool operator>=(const IgPair<TKey, TValue>& pair)
    {
        if (Key != pair.Key) {
            return Key < pair.Key;
        }
        return Value < pair.Value;
    }
};

#endif
