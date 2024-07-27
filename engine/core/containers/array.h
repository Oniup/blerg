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

#ifndef IGNITE_CORE_CONTAINERS__ARRAY_H
#define IGNITE_CORE_CONTAINERS__ARRAY_H

#include "core/common.h"
#include "core/containers/iterator.h"
#include "core/limits.h"
#include "core/memory/utils.h"
#include <cstdlib>

template <typename T, typename TAllocator, size_t TCapacityIncreaseIntervalSize>
struct IgArray {
    using Type          = T;
    using Allocation    = typename TAllocator::template Allocation<T>;
    using Iterator      = IgPackedIterator<T, IgArray>;
    using ConstIterator = IgConstPackedIterator<T, IgArray>;
    static constexpr size_t CapacityIncreaseIntervalSize = TCapacityIncreaseIntervalSize;
    static constexpr size_t NoPos                        = IgNumericLimits<size_t>::Max();

    size_t Size          = 0;
    Allocation Allocator = {};

    constexpr IgArray() = default;

    constexpr IgArray(size_t size)
          : Size(size)
    {
        size_t capacity = Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
        Allocator.Allocate(capacity);
        Ignite::DefaultConstructItems(Begin(), End());
    }

    constexpr IgArray(const std::initializer_list<T>& list)
          : IgArray(list.size())
    {
        Ignite::CopyInitializerListItems(Begin(), list);
    }

    constexpr IgArray(const IgArray& array)
          : Size(array.Size)
    {
        Allocator.Allocate(array.Allocator.Capacity);
        Ignite::CopyItems(Begin(), End(), array.Allocator.Ptr);
    }

    template <typename _OtherAllocator>
    constexpr IgArray(const IgArray<T, _OtherAllocator>& array)
          : Size(array.Size)
    {
        Allocator.Allocate(array.Allocator.Capacity);
        Ignite::CopyItems(Begin(), End(), array.Allocator.Ptr);
    }

    constexpr IgArray(IgArray&& array)
    {
        Allocator = Ignite::Move(array.Allocator);
        Size      = Ignite::Move(array.Size);

        array.Allocator.SetToNullptr();
        array.Size = 0;
    }

    ~IgArray() { Destroy(); }

    constexpr Iterator Begin() { return Iterator(this, 0); }
    constexpr Iterator End() { return Iterator(this, Size); }
    constexpr ConstIterator Begin() const { return ConstIterator(const_cast<IgArray*>(this), 0); }
    constexpr ConstIterator End() const { return ConstIterator(const_cast<IgArray*>(this), Size); }
    constexpr Iterator begin() { return Begin(); }
    constexpr Iterator end() { return End(); }
    constexpr ConstIterator begin() const { return Begin(); }
    constexpr ConstIterator end() const { return End(); }

    constexpr T* Data() { return Allocator.Ptr; }
    constexpr const T* Data() const { return Allocator.Ptr; }
    constexpr size_t Capacity() const { return Allocator.Capacity; }

    constexpr bool IsEmpty() const { return Allocator.IsEmpty() || (Size == 0 || Size == NoPos); }
    constexpr bool IsAllocatorEmpty() const { return Allocator.IsEmpty(); }

    constexpr T& First() { return Allocator.Ptr[0]; }
    constexpr const T& First() const { return Allocator.Ptr[0]; }
    constexpr T& Last() { return Allocator.Ptr[Size - 1]; }
    constexpr const T& Last() const { return Allocator.Ptr[Size - 1]; }

    template <typename _OtherAllocator>
    constexpr IgArray& operator=(const IgArray<T, _OtherAllocator>& array)
    {
        Ignite::DestructItems(Begin(), End());
        Size = array.Size;

        Allocator.ReAllocate(array.Allocator.Capacity);
        Ignite::CopyItems(Begin(), End(), array.Allocator.Ptr);
        return *this;
    }

    constexpr IgArray& operator=(const IgArray& array)
    {
        Ignite::DestructItems(Begin(), End());
        Size = array.Size;

        Allocator.ReAllocate(array.Allocator.Capacity);
        Ignite::CopyItems(Begin(), End(), array.Allocator.Ptr);
        return *this;
    }

    constexpr IgArray& operator=(IgArray&& array)
    {
        Ignite::DestructItems(Begin(), End());
        Allocator = Ignite::Move(array.Allocator);
        Size      = Ignite::Move(array.Size);

        array.Allocator.SetToNullptr();
        array.Size = 0;
        return *this;
    }

    constexpr bool operator!=(const IgArray& array) const { return !(*this == array); }
    constexpr bool operator==(const IgArray& array) const
    {
        if (Size != array.Size) {
            return false;
        }
        for (size_t i = 0; i < Size; i++) {
            if (Allocator.Ptr[i] != array[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr T& operator[](size_t index)
    {
        IG_BASIC_ASSERT(index > Size, "Out of range index");
        return Allocator.Ptr[index];
    }

    constexpr const T& operator[](size_t index) const
    {
        IG_BASIC_ASSERT(index > Size, "Out of range index");
        return Allocator.Ptr[index];
    }

    constexpr void Clear()
    {
        Ignite::DestructItems(Begin(), End());
        Size = 0;
    }

    constexpr void Destroy()
    {
        Clear();
        if (Allocator.Ptr != nullptr) {
            Allocator.Free();
        }
    }

    constexpr void Resize(size_t size)
    {
        if (size < Size) {
            Ignite::DestructItems(Begin() + size, End());
        }
        else {
            ReserveToFit(size);
        }
        Size = size;
    }

    // WARNING: This does not call destructors for existing elements in array.
    constexpr void ReserveToFit(size_t size)
    {
        if (size > Allocator.Capacity) {
            size_t capacity =
                Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
            Reserve(capacity);
        }
    }

    // WARNING: This does not call destructors for existing elements in array.
    constexpr void Reserve(size_t capacity) { Allocator.ReAllocate(capacity); }

    constexpr void PushBack(const T& val)
    {
        size_t pos = Size;
        Resize(Size + 1);
        if constexpr (!std::is_pointer_v<T>) {
            Ignite::CopyConstruct(*(Allocator.Ptr + pos), val);
        }
        else {
            Allocator.Ptr[pos] = val;
        }
    }

    constexpr void PushBack(T&& val)
    {
        size_t pos = Size;
        Resize(Size + 1);
        if constexpr (!std::is_pointer_v<T>) {
            Ignite::MoveConstruct(*(Allocator.Ptr + pos), Ignite::Move(val));
        }
        else {
            Allocator.Ptr[pos] = val;
        }
    }

    template <typename _OtherAllocator>
    constexpr void PushBack(const IgArray<T, _OtherAllocator>& array)
    {
        size_t pos = Size;
        Resize(Size + array.Size);
        Iterator offset = Begin() + pos;
        Ignite::CopyItems(offset, End(), array.Allocator.Ptr);
    }

    constexpr void PushBack(const std::initializer_list<T>& list)
    {
        size_t old_size = Size;
        Resize(Size + list.size());
        Ignite::CopyInitializerListItems(Begin() + old_size, list);
    }

    constexpr void PushFront(const T& val)
    {
        size_t old_size = Size;
        Resize(Size + 1);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + 1);
        if constexpr (!std::is_pointer_v<T>) {
            Ignite::CopyConstruct(Allocator.Ptr[0], val);
        }
        else {
            Allocator.Ptr[0] = val;
        }
    }

    constexpr void PushFront(T&& val)
    {
        size_t old_size = Size;
        Resize(Size + 1);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + 1);
        if constexpr (!std::is_pointer_v<T>) {
            Ignite::MoveConstruct(*Allocator.Ptr, Ignite::Move(val));
        }
        else {
            Allocator.Ptr[0] = Ignite::Move(val);
        }
    }

    template <typename _OtherAllocator>
    constexpr void PushFront(const IgArray<T, _OtherAllocator>& arr)
    {
        size_t old_size = Size;
        Resize(Size + arr.Size);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + arr.Size);
        Ignite::CopyItems(Begin(), Begin() + arr.Size, arr.Allocator.Ptr);
    }

    constexpr void PushFront(const std::initializer_list<T>& list)
    {
        size_t old_size = Size;
        Resize(Size + list.size());
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + list.size());
        Ignite::CopyInitializerListItems(Begin() + old_size, list);
    }

    constexpr void Insert(Iterator pos, T&& val)
    {
        if (pos == End()) {
            PushBack(val);
        }
        else if (pos == Begin()) {
            PushFront(val);
        }
        else {
            size_t old_size = Size;
            Resize(Size + 1);
            Ignite::ShiftItems(pos, Begin() + old_size, pos + 1);
            if constexpr (!std::is_same_v<T, void*>) {
                Ignite::MoveConstruct(*pos, Ignite::Move(val));
            }
            else {
                *pos = Ignite::Move(val);
            }
        }
    }

    template <typename _OtherAllocator>
    constexpr void Insert(Iterator pos, const IgArray<T, _OtherAllocator>& arr)
    {
        if (pos == End()) {
            PushBack(arr);
        }
        else if (pos == Begin()) {
            PushFront(arr);
        }
        else {
            size_t old_size = Size;
            Resize(Size + arr.Size);
            Ignite::ShiftItems(pos, Begin() + old_size, pos + arr.Size);
            Ignite::CopyItems(pos, pos + arr.Size, arr.Allocator.Ptr);
        }
    }

    constexpr void Insert(Iterator pos, const std::initializer_list<T>& list)
    {
        if (pos == End()) {
            PushBack(list);
        }
        else if (pos == Begin()) {
            PushFront(list);
        }
        else {
            size_t old_size = Size;
            Resize(Size + list.size());
            Ignite::ShiftItems(pos, Begin() + old_size, pos + list.size());
            Ignite::CopyInitializerListItems(pos, list);
        }
    }

    constexpr void PopBack(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (Size - size < 1) {
            Clear();
        }
        else {
            Ignite::DestructItems(End() - size, End());
            Resize(Size - size);
        }
    }

    constexpr void PopFront(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (Size - size < 1) {
            Clear();
        }
        else {
            Ignite::DestructItems(Begin(), Begin() + size);
            Ignite::ShiftItems(Begin() + size, End(), Begin());
            Resize(Size - size);
        }
    }

    constexpr void Erase(Iterator begin, Iterator end)
    {
        size_t size = end.Ptr() - begin.Ptr();
        if (size == 0) {
            return;
        }
        if (Size - size < 1) {
            Clear();
        }
        else {
            Ignite::DestructItems(begin, end);
            Ignite::ShiftItems(end, this->End(), begin);
            Resize(Size - size);
        }
    }

    constexpr bool Erase(const T& val)
    {
        size_t offset = Find(val);
        if (offset == NoPos) {
            return false;
        }
        Erase(Begin() + offset, Begin() + offset + 1);
        return true;
    }

    constexpr void Erase(Iterator pos) { PopSlice(pos, pos + 1); }

    constexpr size_t Find(const T& val) const
    {
        for (size_t i = 0; i < Size; ++i) {
            if (Allocator.Ptr[i] == val) {
                return i;
            }
        }
        return NoPos;
    }

    constexpr size_t FindLast(const T& val) const
    {
        for (size_t i = Size - 1; i > 0; --i) {
            if (Allocator.Ptr[i] == val) {
                return i;
            }
        }
        return NoPos;
    }
};

#endif
