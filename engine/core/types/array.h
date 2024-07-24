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

#ifndef IGNITE_CORE_TYPES__ARRAY_H
#define IGNITE_CORE_TYPES__ARRAY_H

#include "core/limits.h"
#include "core/memory_utils.h"
#include "core/types/allocator.h"
#include "core/types/iterator.h"
#include <stdio.h>

#define IG_DEFAULT_ARRAY_CAPACITY_INCREASE_INTERVAL_SIZE 20

template <typename _T, typename _Allocator = IgHeapAllocation,
          size_t _CapacityIncreaseIntervalSize = IG_DEFAULT_ARRAY_CAPACITY_INCREASE_INTERVAL_SIZE>
struct IgArray {
    using Type          = _T;
    using Allocation    = typename _Allocator::template Allocation<_T>;
    using Iterator      = IgPackedIterator<_T, IgArray<_T, _Allocator>>;
    using ConstIterator = IgConstPackedIterator<_T, IgArray<_T, _Allocator>>;
    static constexpr size_t CapacityIncreaseIntervalSize = _CapacityIncreaseIntervalSize;
    static constexpr size_t NoPos                        = IgNumericLimits<size_t>::Max();

    size_t Size          = 0;
    Allocation Allocator = {};

    IgArray() = default;

    IgArray(size_t size) : Size(size)
    {
        size_t capacity = Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
        Allocator.Allocate(capacity);
        Ignite::DefaultConstructItems(Begin(), End());
    }

    IgArray(const std::initializer_list<_T>& list) : IgArray(list.size())
    {
        Ignite::ConstructItemsInitializerList(Begin(), list);
    }

    IgArray(const IgArray& array) : Size(array.Size)
    {
        Allocator.Allocate(array.Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.Allocator.Ptr);
    }

    template <typename _OtherAllocator>
    IgArray(const IgArray<_T, _OtherAllocator>& array) : Size(array.Size)
    {
        Allocator.Allocate(array.Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.Allocator.Ptr);
    }

    IgArray(IgArray&& array)
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
    constexpr ConstIterator end() const { return End; }

    constexpr _T* Data() { return Allocator.Ptr; }
    constexpr const _T* Data() const { return Allocator.Ptr; }

    constexpr _T& First() { return Allocator.Ptr[0]; }
    constexpr const _T& First() const { return Allocator.Ptr[0]; }
    constexpr _T& Last() { return Allocator.Ptr[Size - 1]; }
    constexpr const _T& Last() const { return Allocator.Ptr[Size - 1]; }
    constexpr size_t Capacity() const { return Allocator.Capacity; }

    template <typename _OtherAllocator>
    IgArray& operator=(const IgArray<_T, _OtherAllocator>& array)
    {
        Ignite::DestructItems(Begin(), End());
        Size = array.Size;

        Allocator.ReAllocate(array.Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.Allocator.Ptr);
        return *this;
    }

    IgArray& operator=(const IgArray& array)
    {
        Ignite::DestructItems(Begin(), End());
        Size = array.Size;

        Allocator.ReAllocate(array.Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.Allocator.Ptr);
        return *this;
    }

    IgArray& operator=(IgArray&& array)
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

    constexpr _T& operator[](size_t index)
    {
        IG_BASIC_ASSERT(index > Size, "Out of range index");
        return Allocator.Ptr[index];
    }

    constexpr const _T& operator[](size_t index) const
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

    void PushBack(const _T& val)
    {
        size_t pos = Size;
        Resize(Size + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::CopyConstruct(*(Allocator.Ptr + pos), val);
        }
        else {
            Allocator.Ptr[pos] = val;
        }
    }

    constexpr void PushBack(_T&& val)
    {
        size_t pos = Size;
        Resize(Size + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::MoveConstruct(*(Allocator.Ptr + pos), Ignite::Move(val));
        }
        else {
            Allocator.Ptr[pos] = val;
        }
    }

    template <typename _OtherAllocator>
    constexpr void PushBack(const IgArray<_T, _OtherAllocator>& array)
    {
        size_t pos = Size;
        Resize(Size + array.Size);
        Iterator offset = Begin() + pos;
        Ignite::CopyConstructItems(offset, End(), array.Allocator.Ptr);
    }

    constexpr void PushBack(const std::initializer_list<_T>& list)
    {
        size_t old_size = Size;
        Resize(Size + list.size());
        Ignite::ConstructItemsInitializerList(Begin() + old_size, list);
    }

    void PushFront(const _T& val)
    {
        size_t old_size = Size;
        Resize(Size + 1);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::CopyConstruct(Allocator.Ptr[0], val);
        }
        else {
            Allocator.Ptr[0] = val;
        }
    }

    constexpr void PushFront(_T&& val)
    {
        size_t old_size = Size;
        Resize(Size + 1);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::MoveConstruct(*Allocator.Ptr, Ignite::Move(val));
        }
        else {
            Allocator.Ptr[0] = Ignite::Move(val);
        }
    }

    template <typename _OtherAllocator>
    constexpr void PushFront(const IgArray<_T, _OtherAllocator>& arr)
    {
        size_t old_size = Size;
        Resize(Size + arr.Size);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + arr.Size);
        Ignite::CopyConstructItems(Begin(), Begin() + arr.Size, arr.Allocator.Ptr);
    }

    constexpr void PushFront(const std::initializer_list<_T>& list)
    {
        size_t old_size = Size;
        Resize(Size + list.size());
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + list.size());
        Ignite::ConstructItemsInitializerList(Begin() + old_size, list);
    }

    constexpr void Insert(Iterator pos, _T&& val)
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
            if constexpr (!std::is_same_v<_T, void*>) {
                Ignite::MoveConstruct(*pos, Ignite::Move(val));
            }
            else {
                *pos = Ignite::Move(val);
            }
        }
    }

    template <typename _OtherAllocator>
    constexpr void Insert(Iterator pos, const IgArray<_T, _OtherAllocator>& arr)
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
            Ignite::CopyConstructItems(pos, pos + arr.Size, arr.Allocator.Ptr);
        }
    }

    constexpr void Insert(Iterator pos, const std::initializer_list<_T>& list)
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
            Ignite::ConstructItemsInitializerList(pos, list);
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

    constexpr bool Erase(const _T& val)
    {
        size_t offset = Find(val);
        if (offset == NoPos) {
            return false;
        }
        Erase(Begin() + offset, Begin() + offset + 1);
        return true;
    }

    constexpr void Erase(Iterator pos) { PopSlice(pos, pos + 1); }

    size_t Find(const _T& val) const
    {
        for (size_t i = 0; i < Size; ++i) {
            if (Allocator.Ptr[i] == val) {
                return i;
            }
        }
        return NoPos;
    }

    size_t FindLast(const _T& val) const
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
