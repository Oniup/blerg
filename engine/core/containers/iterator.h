// This file is part of Fiwre (https://github.com/oniup/fiwre)
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

#ifndef CORE_CONTAINERS__ITERATOR_H
#define CORE_CONTAINERS__ITERATOR_H

#include <cstdio>

namespace fiwre {

template <typename T, typename TPackedContainer>
struct PackedIterator {
  using Type            = T;
  using PackedContainer = TPackedContainer;

  constexpr PackedIterator() = default;
  constexpr PackedIterator(PackedContainer* container, size_t offset)
    : _container(container), _offset(offset) {}

  inline T* ptr() { return _container->data() + _offset; }
  inline const T* ptr() const { return _container->data() + _offset; }

  constexpr T& operator*() { return *ptr(); }
  constexpr T* operator->() { return ptr(); }
  constexpr const T& operator*() const { return *ptr(); }
  constexpr const T* operator->() const { return ptr(); }

  constexpr bool operator==(const PackedIterator& iter) const {
    return ptr() == iter.ptr();
  }
  constexpr bool operator!=(const PackedIterator& iter) const {
    return ptr() != iter.ptr();
  }

  constexpr PackedIterator& operator++() {
    _offset++;
    return *this;
  }

  constexpr PackedIterator operator++(int) {
    PackedIterator temp = *this;
    ++(*this);
    return temp;
  }

  constexpr PackedIterator& operator--() {
    _offset--;
    return *this;
  }

  constexpr PackedIterator operator--(int) {
    PackedIterator temp = *this;
    --(*this);
    return temp;
  }

  constexpr PackedIterator operator+(size_t offset) const {
    return PackedIterator(_container, _offset + offset);
  }

  constexpr PackedIterator operator-(size_t offset) const {
    return PackedIterator(_container, _offset - offset);
  }

  constexpr friend PackedIterator operator+(size_t offset,
                                            const PackedIterator& iter) {
    return PackedIterator(iter._container, iter._offset + offset);
  }

  constexpr friend PackedIterator operator-(size_t offset,
                                            const PackedIterator& iter) {
    return PackedIterator(iter._container, iter._offset - offset);
  }

private:
  PackedContainer* _container = nullptr;
  size_t _offset              = 0;
};

template <typename T, typename TPackedContainer>
struct ConstPackedIterator {
  using Type            = T;
  using PackedContainer = TPackedContainer;

  constexpr ConstPackedIterator() = default;
  constexpr ConstPackedIterator(PackedContainer* container, size_t offset)
    : _container(container), _offset(offset) {}

  inline const T* ptr() const { return _container->data() + _offset; }

  constexpr T& operator*() { return *ptr(); }
  constexpr T* operator->() { return ptr(); }
  constexpr const T& operator*() const { return *ptr(); }
  constexpr const T* operator->() const { return ptr(); }

  constexpr bool operator==(const ConstPackedIterator& iter) const {
    return ptr() == iter.ptr();
  }

  constexpr bool operator!=(const ConstPackedIterator& iter) const {
    return ptr() != iter.ptr();
  }

  constexpr ConstPackedIterator& operator++() {
    _offset++;
    return *this;
  }

  constexpr ConstPackedIterator operator++(int) {
    ConstPackedIterator temp = *this;
    ++(*this);
    return temp;
  }

  constexpr ConstPackedIterator& operator--() {
    _offset--;
    return *this;
  }

  constexpr ConstPackedIterator operator--(int) {
    ConstPackedIterator temp = *this;
    --(*this);
    return temp;
  }

  constexpr ConstPackedIterator operator+(size_t offset) const {
    return ConstPackedIterator(_container, _offset + offset);
  }

  constexpr ConstPackedIterator operator-(size_t offset) const {
    return ConstPackedIterator(_container, _offset - offset);
  }

  constexpr friend ConstPackedIterator
      operator+(size_t offset, const ConstPackedIterator& iter) {
    return ConstPackedIterator(iter._container, iter._offset + offset);
  }

  constexpr friend ConstPackedIterator
      operator-(size_t offset, const ConstPackedIterator& iter) {
    return ConstPackedIterator(iter._container, iter._offset - offset);
  }

private:
  PackedContainer* _container = nullptr;
  size_t _offset              = 0;
};

} // namespace fiwre

#endif
