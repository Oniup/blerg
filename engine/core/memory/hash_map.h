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

#ifndef IGNITE_CORE_CONTAINERS__HASH_MAP_H
#define IGNITE_CORE_CONTAINERS__HASH_MAP_H

#include "core/containers/array.h"
#include "core/containers/pair.h"
#include "core/functional.h"
#include "core/memory/allocator.h"

template <typename TKey, typename TValue, typename TAllocator>
struct IgHashMapElement {
    using Allocation = typename TAllocator::template Allocation<IgHashMapElement>;

    Allocation Next            = nullptr;
    IgHashMapElement* Previous = nullptr;
    IgPair<TKey, TValue> Data;

    IgHashMapElement()
    {
        if (Next != nullptr) {
            Ignite::Destruct(Next);
            Next.Free();
        }
    }

    inline bool operator==(const IgHashMapElement& element) const
    {
        if (Size() != element.Size()) {
            return false;
        }
        return Compare(this, &element);
    }

    inline bool operator!=(const IgHashMapElement& element) const { return !(*this == element); }

    IgHashMapElement* Last()
    {
        if (Next != nullptr) {
            return Next.Last();
        }
        return this;
    }

    size_t Size() const
    {
        if (Next != nullptr) {
            return 1 + Next.Size();
        }
        return 1;
    }

    TValue* Value(const TKey& key)
    {
        if (Data.Key == key) {
            return &Data.Value;
        }
        if (Next != nullptr) {
            return Next.Value(key);
        }
        return nullptr;
    }

    inline const TValue* Value(const TKey& key) const { return Value(key); }
    inline const TKey& Key() const { return Data.Key; }

    static bool Compare(const IgHashMapElement* element, const IgHashMapElement* compare)
    {
        bool exists = element != nullptr;
        if (exists) {
            if (compare != nullptr && element->Data == compare->Data) {
                return Compare(element->Next.Ptr, compare->Next.Ptr);
            }
        }
        else if (compare == nullptr) {
            return true;
        }
        return false;
    }
};

template <typename TKey, typename TValue, typename THasher = IgHash<TKey>,
          typename TComparator = IgHash<TKey>, typename TAllocator = IgHeapAllocation,
          typename TElementAllocator           = IgHeapAllocation,
          size_t TCapacityIncreaseIntervalSize = IG_DEFAULT_CAPACITY_INTERVAL>
struct IgHashMap {
    using KeyType    = TKey;
    using ValueType  = TValue;
    using Element    = IgHashMapElement<TKey, TValue, TElementAllocator>;
    using Allocation = typename TAllocator::template Allocation<Element>;
    static constexpr size_t CapacityIncreaseIntervalSize = TCapacityIncreaseIntervalSize;

    IgArray<Allocation, TAllocator, TCapacityIncreaseIntervalSize> Elements = {};

    constexpr IgHashMap() = default;
    ~IgHashMap()          = default;

    constexpr IgHashMap(const IgHashMap& map)
          : Elements(map.Elements)
    {
    }

    constexpr IgHashMap(IgHashMap&& map)
          : Elements(Ignite::Move(map.Elements))
    {
    }

    constexpr bool Contains(const TKey& key) const { return TryAt(key) != nullptr; }

    constexpr TValue* TryAt(const TKey& key)
    {
        uint64_t hash    = _CalcHash(key);
        Element* element = Elements[hash].Ptr;
        TValue* val      = element->Value(hash);
        if (val != nullptr) {
            return val;
        }
        return val;
    }

    constexpr const TValue* TryAt(const TKey& key) const
    {
        uint64_t hash    = _CalcHash(key);
        Element* element = Elements[hash].Ptr;
        TValue* val      = element->Value(hash);
        if (val != nullptr) {
            return val;
        }
        return val;
    }

    constexpr TValue& At(const TKey& key)
    {
        TValue* value = TryAt(key);
        IG_BASIC_ASSERT(value == nullptr, "Key not created yet. Invalid data access");
        return value;
    }

    constexpr const TValue& At(const TKey& key) const
    {
        TValue* value = TryAt(key);
        IG_BASIC_ASSERT(value == nullptr, "Key not created yet. Invalid data access");
        return value;
    }

    constexpr TValue& operator[](const TKey& key) {}

    constexpr bool operator==(const IgHashMap& map) const
    {
        if (Elements.Size != map.Elements.Size && Elements.Capacity() != map.Elements.Capacity() &&
            (Elements.IsEmpty() != nullptr) != (map.Elements.IsEmpty() != nullptr)) {
            return false;
        }
        for (size_t i = 0; i < Elements.Capacity(); i++) {
            bool exists = Elements[i] != nullptr;
            if (exists == (map.Elements[i] != nullptr)) {
                if (exists && *Elements[i] != *map.Elements[i]) {
                    return false;
                }
            }
        }
        return true;
    }

    constexpr bool operator!=(const IgHashMap& map) const { return !(*this == map); }

    constexpr void Remove(const TKey& key) {}

    constexpr void Clear() {}

private:
    constexpr uint64_t _CalcHash(const TKey& key) { return THasher::Hash(key); }

    constexpr bool _GrowthRequired(size_t size_increase)
    {
        return size_increase + Elements.Size >
               Elements.Capacity() + TCapacityIncreaseIntervalSize * 0.2;
    }

    constexpr Element* InsertElement()
    {
        if (_GrowthRequired(1)) {
            _ResizeAndReHash(Elements.Capacity() + Elements.CapacityIncreaseIntervalSize);
        }
    }

    constexpr void _ResizeAndReHash(size_t new_capacity_size)
    {
        IgArray tmp(Ignite::Move(Elements));
        Elements.Resize(new_capacity_size);

        for (Allocation& element_alloc : tmp) {
            Element* ptr = element_alloc.Ptr;
        }
    }
};

#endif
