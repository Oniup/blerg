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

#ifndef IGNITE_CORE_TYPES__ITERATOR_H
#define IGNITE_CORE_TYPES__ITERATOR_H

#include "core/common.h"

template <typename _T, typename _PackedContainer>
struct IgPackedIterator {
    using Type            = _T;
    using PackedContainer = _PackedContainer;

    constexpr IgPackedIterator() = default;
    constexpr IgPackedIterator(PackedContainer* container, size_t offset)
          : m_Container(container), m_Offset(offset)
    {
    }

    inline _T* Ptr() { return m_Container->Data() + m_Offset; }
    inline const _T* Ptr() const { return m_Container->Data() + m_Offset; }

    constexpr _T& operator*() { return *Ptr(); }
    constexpr _T* operator->() { return Ptr(); }
    constexpr const _T& operator*() const { return *Ptr(); }
    constexpr const _T* operator->() const { return Ptr(); }

    constexpr bool operator==(const IgPackedIterator& iter) const { return Ptr() == iter.Ptr(); }
    constexpr bool operator!=(const IgPackedIterator& iter) const { return Ptr() != iter.Ptr(); }

    constexpr IgPackedIterator& operator++()
    {
        m_Offset++;
        return *this;
    }

    constexpr IgPackedIterator operator++(int)
    {
        IgPackedIterator temp = *this;
        ++(*this);
        return temp;
    }

    constexpr IgPackedIterator& operator--()
    {
        m_Offset--;
        return *this;
    }

    constexpr IgPackedIterator operator--(int)
    {
        IgPackedIterator temp = *this;
        --(*this);
        return temp;
    }

    constexpr IgPackedIterator operator+(size_t offset) const
    {
        return IgPackedIterator(m_Container, m_Offset + offset);
    }

    constexpr IgPackedIterator operator-(size_t offset) const
    {
        return IgPackedIterator(m_Container, m_Offset - offset);
    }

    constexpr friend IgPackedIterator operator+(size_t offset, const IgPackedIterator& iter)
    {
        return IgPackedIterator(iter.m_Container, iter.m_Offset + offset);
    }

    constexpr friend IgPackedIterator operator-(size_t offset, const IgPackedIterator& iter)
    {
        return IgPackedIterator(iter.m_Container, iter.m_Offset - offset);
    }

private:
    PackedContainer* m_Container = nullptr;
    size_t m_Offset              = 0;
};

template <typename _T, typename _PackedContainer>
struct IgConstPackedIterator {
    using Type            = _T;
    using PackedContainer = _PackedContainer;

    constexpr IgConstPackedIterator() = default;
    constexpr IgConstPackedIterator(PackedContainer* container, size_t offset)
          : m_Container(container), m_Offset(offset)
    {
    }

    inline const _T* Ptr() const { return m_Container->Data() + m_Offset; }

    constexpr _T& operator*() { return *Ptr(); }
    constexpr _T* operator->() { return Ptr(); }
    constexpr const _T& operator*() const { return *Ptr(); }
    constexpr const _T* operator->() const { return Ptr(); }

    constexpr bool operator==(const IgConstPackedIterator& iter) const
    {
        return Ptr() == iter.Ptr();
    }

    constexpr bool operator!=(const IgConstPackedIterator& iter) const
    {
        return Ptr() != iter.Ptr();
    }

    constexpr IgConstPackedIterator& operator++()
    {
        m_Offset++;
        return *this;
    }

    constexpr IgConstPackedIterator operator++(int)
    {
        IgConstPackedIterator temp = *this;
        ++(*this);
        return temp;
    }

    constexpr IgConstPackedIterator& operator--()
    {
        m_Offset--;
        return *this;
    }

    constexpr IgConstPackedIterator operator--(int)
    {
        IgConstPackedIterator temp = *this;
        --(*this);
        return temp;
    }

    constexpr IgConstPackedIterator operator+(size_t offset) const
    {
        return IgConstPackedIterator(m_Container, m_Offset + offset);
    }

    constexpr IgConstPackedIterator operator-(size_t offset) const
    {
        return IgConstPackedIterator(m_Container, m_Offset - offset);
    }

    constexpr friend IgConstPackedIterator operator+(size_t offset,
                                                     const IgConstPackedIterator& iter)
    {
        return IgConstPackedIterator(iter.m_Container, iter.m_Offset + offset);
    }

    constexpr friend IgConstPackedIterator operator-(size_t offset,
                                                     const IgConstPackedIterator& iter)
    {
        return IgConstPackedIterator(iter.m_Container, iter.m_Offset - offset);
    }

private:
    PackedContainer* m_Container = nullptr;
    size_t m_Offset              = 0;
};

#endif
