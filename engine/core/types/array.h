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

#include "core/common.h"
#include "core/limits.h"
#include "core/memory_utils.h"
#include "core/types/iterator.h"
#include <cstdlib>

template <typename _T, typename _Allocator, size_t _CapacityIncreaseIntervalSize>
struct IgArray {
    using Type          = _T;
    using Allocation    = typename _Allocator::template Allocation<_T>;
    using Iterator      = IgPackedIterator<_T, IgArray>;
    using ConstIterator = IgConstPackedIterator<_T, IgArray>;
    static constexpr size_t CapacityIncreaseIntervalSize = _CapacityIncreaseIntervalSize;
    static constexpr size_t NoPos                        = IgNumericLimits<size_t>::Max();

    constexpr IgArray() = default;

    constexpr IgArray(size_t size)
          : m_Size(size)
    {
        size_t capacity = m_Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
        m_Allocator.Allocate(capacity);
        Ignite::DefaultConstructItems(Begin(), End());
    }

    constexpr IgArray(const std::initializer_list<_T>& list)
          : IgArray(list.size())
    {
        Ignite::ConstructItemsInitializerList(Begin(), list);
    }

    constexpr IgArray(const IgArray& array)
          : m_Size(array.m_Size)
    {
        m_Allocator.Allocate(array.m_Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.m_Allocator.Ptr);
    }

    template <typename _OtherAllocator>
    constexpr IgArray(const IgArray<_T, _OtherAllocator>& array)
          : m_Size(array.m_Size)
    {
        m_Allocator.Allocate(array.m_Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.m_Allocator.Ptr);
    }

    constexpr IgArray(IgArray&& array)
    {
        m_Allocator = Ignite::Move(array.m_Allocator);
        m_Size      = Ignite::Move(array.m_Size);

        array.m_Allocator.SetToNullptr();
        array.m_Size = 0;
    }

    ~IgArray() { Destroy(); }

    constexpr Iterator Begin() { return Iterator(this, 0); }
    constexpr Iterator End() { return Iterator(this, m_Size); }
    constexpr ConstIterator Begin() const { return ConstIterator(const_cast<IgArray*>(this), 0); }
    constexpr ConstIterator End() const
    {
        return ConstIterator(const_cast<IgArray*>(this), m_Size);
    }
    constexpr Iterator begin() { return Begin(); }
    constexpr Iterator end() { return End(); }
    constexpr ConstIterator begin() const { return Begin(); }
    constexpr ConstIterator end() const { return End(); }

    constexpr _T* Data() { return m_Allocator.Ptr; }
    constexpr const _T* Data() const { return m_Allocator.Ptr; }
    constexpr size_t Size() const { return m_Size; }
    constexpr size_t Capacity() const { return m_Allocator.Capacity; }

    constexpr bool IsEmpty() const
    {
        return m_Allocator.IsEmpty() || (m_Size == 0 || m_Size == NoPos);
    }
    constexpr bool IsAllocatorEmpty() const { return m_Allocator.IsEmpty(); }

    constexpr _T& First() { return m_Allocator.Ptr[0]; }
    constexpr const _T& First() const { return m_Allocator.Ptr[0]; }
    constexpr _T& Last() { return m_Allocator.Ptr[m_Size - 1]; }
    constexpr const _T& Last() const { return m_Allocator.Ptr[m_Size - 1]; }

    template <typename _OtherAllocator>
    constexpr IgArray& operator=(const IgArray<_T, _OtherAllocator>& array)
    {
        Ignite::DestructItems(Begin(), End());
        m_Size = array.m_Size;

        m_Allocator.ReAllocate(array.m_Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.m_Allocator.Ptr);
        return *this;
    }

    constexpr IgArray& operator=(const IgArray& array)
    {
        Ignite::DestructItems(Begin(), End());
        m_Size = array.m_Size;

        m_Allocator.ReAllocate(array.m_Allocator.Capacity);
        Ignite::CopyConstructItems(Begin(), End(), array.m_Allocator.Ptr);
        return *this;
    }

    constexpr IgArray& operator=(IgArray&& array)
    {
        Ignite::DestructItems(Begin(), End());
        m_Allocator = Ignite::Move(array.m_Allocator);
        m_Size      = Ignite::Move(array.m_Size);

        array.m_Allocator.SetToNullptr();
        array.m_Size = 0;
        return *this;
    }

    constexpr bool operator!=(const IgArray& array) const { return !(*this == array); }
    constexpr bool operator==(const IgArray& array) const
    {
        if (m_Size != array.m_Size) {
            return false;
        }
        for (size_t i = 0; i < m_Size; i++) {
            if (m_Allocator.Ptr[i] != array[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr _T& operator[](size_t index)
    {
        IG_BASIC_ASSERT(index > m_Size, "Out of range index");
        return m_Allocator.Ptr[index];
    }

    constexpr const _T& operator[](size_t index) const
    {
        IG_BASIC_ASSERT(index > m_Size, "Out of range index");
        return m_Allocator.Ptr[index];
    }

    constexpr void Clear()
    {
        Ignite::DestructItems(Begin(), End());
        m_Size = 0;
    }

    constexpr void Destroy()
    {
        Clear();
        if (m_Allocator.Ptr != nullptr) {
            m_Allocator.Free();
        }
    }

    constexpr void Resize(size_t size)
    {
        if (size < m_Size) {
            Ignite::DestructItems(Begin() + size, End());
        }
        else {
            ReserveToFit(size);
        }
        m_Size = size;
    }

    // WARNING: This does not call destructors for existing elements in array.
    constexpr void ReserveToFit(size_t size)
    {
        if (size > m_Allocator.Capacity) {
            size_t capacity =
                m_Allocator.CalcRequiredCapacitySize(size, CapacityIncreaseIntervalSize);
            Reserve(capacity);
        }
    }

    // WARNING: This does not call destructors for existing elements in array.
    constexpr void Reserve(size_t capacity) { m_Allocator.ReAllocate(capacity); }

    constexpr void PushBack(const _T& val)
    {
        size_t pos = m_Size;
        Resize(m_Size + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::CopyConstruct(*(m_Allocator.Ptr + pos), val);
        }
        else {
            m_Allocator.Ptr[pos] = val;
        }
    }

    constexpr void PushBack(_T&& val)
    {
        size_t pos = m_Size;
        Resize(m_Size + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::MoveConstruct(*(m_Allocator.Ptr + pos), Ignite::Move(val));
        }
        else {
            m_Allocator.Ptr[pos] = val;
        }
    }

    template <typename _OtherAllocator>
    constexpr void PushBack(const IgArray<_T, _OtherAllocator>& array)
    {
        size_t pos = m_Size;
        Resize(m_Size + array.m_Size);
        Iterator offset = Begin() + pos;
        Ignite::CopyConstructItems(offset, End(), array.m_Allocator.Ptr);
    }

    constexpr void PushBack(const std::initializer_list<_T>& list)
    {
        size_t old_size = m_Size;
        Resize(m_Size + list.size());
        Ignite::ConstructItemsInitializerList(Begin() + old_size, list);
    }

    constexpr void PushFront(const _T& val)
    {
        size_t old_size = m_Size;
        Resize(m_Size + 1);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::CopyConstruct(m_Allocator.Ptr[0], val);
        }
        else {
            m_Allocator.Ptr[0] = val;
        }
    }

    constexpr void PushFront(_T&& val)
    {
        size_t old_size = m_Size;
        Resize(m_Size + 1);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + 1);
        if constexpr (!std::is_pointer_v<_T>) {
            Ignite::MoveConstruct(*m_Allocator.Ptr, Ignite::Move(val));
        }
        else {
            m_Allocator.Ptr[0] = Ignite::Move(val);
        }
    }

    template <typename _OtherAllocator>
    constexpr void PushFront(const IgArray<_T, _OtherAllocator>& arr)
    {
        size_t old_size = m_Size;
        Resize(m_Size + arr.m_Size);
        Ignite::ShiftItems(Begin(), Begin() + old_size, Begin() + arr.m_Size);
        Ignite::CopyConstructItems(Begin(), Begin() + arr.m_Size, arr.m_Allocator.Ptr);
    }

    constexpr void PushFront(const std::initializer_list<_T>& list)
    {
        size_t old_size = m_Size;
        Resize(m_Size + list.size());
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
            size_t old_size = m_Size;
            Resize(m_Size + 1);
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
            size_t old_size = m_Size;
            Resize(m_Size + arr.m_Size);
            Ignite::ShiftItems(pos, Begin() + old_size, pos + arr.m_Size);
            Ignite::CopyConstructItems(pos, pos + arr.m_Size, arr.m_Allocator.Ptr);
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
            size_t old_size = m_Size;
            Resize(m_Size + list.size());
            Ignite::ShiftItems(pos, Begin() + old_size, pos + list.size());
            Ignite::ConstructItemsInitializerList(pos, list);
        }
    }

    constexpr void PopBack(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (m_Size - size < 1) {
            Clear();
        }
        else {
            Ignite::DestructItems(End() - size, End());
            Resize(m_Size - size);
        }
    }

    constexpr void PopFront(size_t size = 1)
    {
        if (size == 0) {
            return;
        }
        if (m_Size - size < 1) {
            Clear();
        }
        else {
            Ignite::DestructItems(Begin(), Begin() + size);
            Ignite::ShiftItems(Begin() + size, End(), Begin());
            Resize(m_Size - size);
        }
    }

    constexpr void Erase(Iterator begin, Iterator end)
    {
        size_t size = end.Ptr() - begin.Ptr();
        if (size == 0) {
            return;
        }
        if (m_Size - size < 1) {
            Clear();
        }
        else {
            Ignite::DestructItems(begin, end);
            Ignite::ShiftItems(end, this->End(), begin);
            Resize(m_Size - size);
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

    constexpr size_t Find(const _T& val) const
    {
        for (size_t i = 0; i < m_Size; ++i) {
            if (m_Allocator.Ptr[i] == val) {
                return i;
            }
        }
        return NoPos;
    }

    constexpr size_t FindLast(const _T& val) const
    {
        for (size_t i = m_Size - 1; i > 0; --i) {
            if (m_Allocator.Ptr[i] == val) {
                return i;
            }
        }
        return NoPos;
    }

private:
    size_t m_Size          = 0;
    Allocation m_Allocator = {};
};

#endif
