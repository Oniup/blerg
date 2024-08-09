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

#ifndef CORE_CONTAINERS__ARRAY_H
#define CORE_CONTAINERS__ARRAY_H

#include "core/containers/iterator.h"
#include "core/defines.h"
#include "core/limits.h"
#include "core/memory/allocator.h"
#include "core/memory/utils.h"
#include <cstdlib>

namespace fiwre {

template <typename T, typename TAllocator = HeapAllocator,
          size_t TCapacityIncreaseIntervalSize = DEFAULT_CAPACITY_INTERVAL>
struct Array {
  using Type          = T;
  using Allocation    = typename TAllocator::template Allocation<T>;
  using Iterator      = PackedIterator<T, Array>;
  using ConstIterator = ConstPackedIterator<T, Array>;
  static constexpr size_t CapacityIncreaseIntervalSize =
      TCapacityIncreaseIntervalSize;
  static constexpr size_t NoPos = NumericLimits<size_t>::max();

  constexpr Array() = default;

  constexpr Array(size_t size)
    : _size(size) {
    size_t capacity = _allocator.calc_required_capacity_size(
        size, CapacityIncreaseIntervalSize);
    _allocator.allocate(capacity);
    fiwre::default_construct_items(begin(), end());
  }

  constexpr Array(const std::initializer_list<T>& list)
    : Array(list.size()) {
    fiwre::copy_initializer_list_items(begin(), list);
  }

  constexpr Array(const Array& array)
    : _size(array._size) {
    _allocator.allocate(array._allocator.capacity);
    fiwre::copy_items(begin(), end(), array._allocator.ptr);
  }

  template <typename TOtherAllocator>
  constexpr Array(const Array<T, TOtherAllocator>& array)
    : _size(array._size) {
    _allocator.allocate(array._allocator.capacity);
    fiwre::copy_items(begin(), end(), array._allocator.ptr);
  }

  constexpr Array(Array&& array) {
    _allocator = fiwre::move(array._allocator);
    _size      = fiwre::move(array._size);

    array._allocator.set_to_nullptr();
    array._size = 0;
  }

  ~Array() { destroy(); }

  constexpr Iterator begin() { return Iterator(this, 0); }
  constexpr Iterator end() { return Iterator(this, _size); }
  constexpr ConstIterator begin() const {
    return ConstIterator(const_cast<Array*>(this), 0);
  }
  constexpr ConstIterator end() const {
    return ConstIterator(const_cast<Array*>(this), _size);
  }

  constexpr T* data() { return _allocator.ptr; }
  constexpr const T* data() const { return _allocator.ptr; }
  constexpr size_t size() const { return _size; }
  constexpr size_t capacity() const { return _allocator.capacity; }

  constexpr bool is_empty() const {
    return _allocator.is_empty() || (_size == 0 || _size == NoPos);
  }
  constexpr bool is_allocator_empty() const { return _allocator.is_empty(); }

  constexpr T& first() { return _allocator.ptr[0]; }
  constexpr const T& first() const { return _allocator.ptr[0]; }
  constexpr T& last() { return _allocator.ptr[_size - 1]; }
  constexpr const T& last() const { return _allocator.ptr[_size - 1]; }

  template <typename TOtherAllocator>
  constexpr Array& operator=(const Array<T, TOtherAllocator>& array) {
    fiwre::destruct_items(begin(), end());
    _size = array._size;

    _allocator.reallocate(array._allocator.capacity);
    fiwre::copy_items(begin(), end(), array._allocator.ptr);
    return *this;
  }

  constexpr Array& operator=(const Array& array) {
    fiwre::destruct_items(begin(), end());
    _size = array._size;

    _allocator.reallocate(array._allocator.capacity);
    fiwre::copy_items(begin(), end(), array._allocator.ptr);
    return *this;
  }

  constexpr Array& operator=(Array&& array) {
    fiwre::destruct_items(begin(), end());
    _allocator = fiwre::move(array._allocator);
    _size      = fiwre::move(array._size);

    array._allocator.set_to_nullptr();
    array._size = 0;
    return *this;
  }

  constexpr bool operator!=(const Array& array) const {
    return !(*this == array);
  }
  constexpr bool operator==(const Array& array) const {
    if (_size != array._size) {
      return false;
    }
    for (size_t i = 0; i < _size; i++) {
      if (_allocator.ptr[i] != array[i]) {
        return false;
      }
    }
    return true;
  }

  constexpr T& operator[](size_t index) {
    BASIC_ASSERT(index > _size, "Out of range index");
    return _allocator.ptr[index];
  }

  constexpr const T& operator[](size_t index) const {
    BASIC_ASSERT(index > _size, "Out of range index");
    return _allocator.ptr[index];
  }

  constexpr void clear() {
    fiwre::destruct_items(begin(), end());
    _size = 0;
  }

  constexpr void destroy() {
    clear();
    if (_allocator.ptr != nullptr) {
      _allocator.free();
    }
  }

  constexpr void resize(size_t size) {
    if (size < _size) {
      fiwre::destruct_items(begin() + size, end());
    } else {
      reserve_to_fit(size);
    }
    _size = size;
  }

  // WARNING: This does not call destructors for existing elements in array.
  constexpr void reserve_to_fit(size_t size) {
    if (size > _allocator.capacity) {
      size_t capacity = _allocator.calc_required_capacity_size(
          size, CapacityIncreaseIntervalSize);
      reserve(capacity);
    }
  }

  // WARNING: This does not call destructors for existing elements in array.
  constexpr void reserve(size_t capacity) { _allocator.reallocate(capacity); }

  constexpr void push_back(const T& val) {
    size_t pos = _size;
    resize(_size + 1);
    if constexpr (!std::is_pointer_v<T>) {
      fiwre::copy_construct(*(_allocator.ptr + pos), val);
    } else {
      _allocator.ptr[pos] = val;
    }
  }

  constexpr void push_back(T&& val) {
    size_t pos = _size;
    resize(_size + 1);
    if constexpr (!std::is_pointer_v<T>) {
      fiwre::move_construct(*(_allocator.ptr + pos), fiwre::move(val));
    } else {
      _allocator.ptr[pos] = val;
    }
  }

  template <typename TOtherAllocator>
  constexpr void push_back(const Array<T, TOtherAllocator>& array) {
    size_t pos = _size;
    resize(_size + array._size);
    Iterator offset = begin() + pos;
    fiwre::copy_items(offset, end(), array._allocator.ptr);
  }

  constexpr void push_back(const std::initializer_list<T>& list) {
    size_t old_size = _size;
    resize(_size + list.size());
    fiwre::copy_initializer_list_items(begin() + old_size, list);
  }

  constexpr void push_front(const T& val) {
    size_t old_size = _size;
    resize(_size + 1);
    fiwre::shift_items(begin(), begin() + old_size, begin() + 1);
    if constexpr (!std::is_pointer_v<T>) {
      fiwre::copy_construct(_allocator.ptr[0], val);
    } else {
      _allocator.ptr[0] = val;
    }
  }

  constexpr void push_front(T&& val) {
    size_t old_size = _size;
    resize(_size + 1);
    fiwre::shift_items(begin(), begin() + old_size, begin() + 1);
    if constexpr (!std::is_pointer_v<T>) {
      fiwre::move_construct(*_allocator.ptr, fiwre::move(val));
    } else {
      _allocator.ptr[0] = fiwre::move(val);
    }
  }

  template <typename TOtherAllocator>
  constexpr void PushFront(const Array<T, TOtherAllocator>& arr) {
    size_t old_size = _size;
    resize(_size + arr._size);
    fiwre::shift_items(begin(), begin() + old_size, begin() + arr._size);
    fiwre::copy_items(begin(), begin() + arr._size, arr._allocator.ptr);
  }

  constexpr void push_front(const std::initializer_list<T>& list) {
    size_t old_size = _size;
    resize(_size + list.size());
    fiwre::shift_items(begin(), begin() + old_size, begin() + list.size());
    fiwre::copy_initializer_list_items(begin() + old_size, list);
  }

  constexpr void insert(Iterator pos, T&& val) {
    if (pos == end()) {
      push_back(val);
    } else if (pos == begin()) {
      push_front(val);
    } else {
      size_t old_size = _size;
      resize(_size + 1);
      fiwre::shift_items(pos, begin() + old_size, pos + 1);
      if constexpr (!std::is_same_v<T, void*>) {
        fiwre::move_construct(*pos, fiwre::move(val));
      } else {
        *pos = fiwre::move(val);
      }
    }
  }

  template <typename TOtherAllocator>
  constexpr void insert(Iterator pos, const Array<T, TOtherAllocator>& arr) {
    if (pos == end()) {
      push_back(arr);
    } else if (pos == begin()) {
      push_front(arr);
    } else {
      size_t old_size = _size;
      resize(_size + arr._size);
      fiwre::shift_items(pos, begin() + old_size, pos + arr._size);
      fiwre::copy_items(pos, pos + arr._size, arr._allocator.ptr);
    }
  }

  constexpr void insert(Iterator pos, const std::initializer_list<T>& list) {
    if (pos == end()) {
      push_back(list);
    } else if (pos == begin()) {
      push_front(list);
    } else {
      size_t old_size = _size;
      resize(_size + list.size());
      fiwre::shift_items(pos, begin() + old_size, pos + list.size());
      fiwre::copy_initializer_list_items(pos, list);
    }
  }

  constexpr void pop_back(size_t size = 1) {
    if (size == 0) {
      return;
    }
    if (_size - size < 1) {
      clear();
    } else {
      fiwre::destruct_items(end() - size, end());
      resize(_size - size);
    }
  }

  constexpr void pop_front(size_t size = 1) {
    if (size == 0) {
      return;
    }
    if (_size - size < 1) {
      clear();
    } else {
      fiwre::destruct_items(begin(), begin() + size);
      fiwre::shift_items(begin() + size, end(), begin());
      resize(_size - size);
    }
  }

  constexpr void erase(Iterator begin, Iterator end) {
    size_t size = end.ptr() - begin.ptr();
    if (size == 0) {
      return;
    }
    if (_size - size < 1) {
      clear();
    } else {
      fiwre::destruct_items(begin, end);
      fiwre::shift_items(end, this->end(), begin);
      resize(_size - size);
    }
  }

  constexpr bool erase(const T& val) {
    size_t offset = find(val);
    if (offset == NoPos) {
      return false;
    }
    erase(begin() + offset, begin() + offset + 1);
    return true;
  }

  constexpr void erase(Iterator pos) { erase(pos, pos + 1); }

  constexpr size_t find(const T& val) const {
    for (size_t i = 0; i < _size; ++i) {
      if (_allocator.ptr[i] == val) {
        return i;
      }
    }
    return NoPos;
  }

  constexpr size_t find_last(const T& val) const {
    for (size_t i = _size - 1; i > 0; --i) {
      if (_allocator.ptr[i] == val) {
        return i;
      }
    }
    return NoPos;
  }

private:
  size_t _size          = 0;
  Allocation _allocator = {};
};

} // namespace fiwre

#endif
