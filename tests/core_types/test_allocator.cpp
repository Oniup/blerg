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

#include "core/memory/allocator.h"
#include <gtest/gtest.h>

template <typename _T>
class HeapAllocationTest : public ::testing::Test {
protected:
    typename blerg::HeapAllocator::template Allocation<_T> alloc;
};

using TestTypes = testing::Types<char, int, double>;
TYPED_TEST_CASE(HeapAllocationTest, TestTypes);

TYPED_TEST(HeapAllocationTest, AllocateTest)
{
    this->alloc.allocate(10);

    ASSERT_NE(this->alloc.ptr, nullptr);
    ASSERT_EQ(this->alloc.capacity, 10);

    this->alloc.free();
    ASSERT_EQ(this->alloc.capacity, 0);
    ASSERT_EQ(this->alloc.ptr, nullptr);
}

TYPED_TEST(HeapAllocationTest, ReAllocateTest)
{
    this->alloc.allocate(10);
    this->alloc.reallocate(20);

    ASSERT_NE(this->alloc.ptr, nullptr);
    ASSERT_EQ(this->alloc.capacity, 20);

    this->alloc.free();
    ASSERT_EQ(this->alloc.capacity, 0);
    ASSERT_EQ(this->alloc.ptr, nullptr);
}

TYPED_TEST(HeapAllocationTest, ReAllocateWithoutInitialAllocationTest)
{
    this->alloc.reallocate(20);

    ASSERT_NE(this->alloc.ptr, nullptr);
    ASSERT_EQ(this->alloc.capacity, 20);

    this->alloc.free();
    ASSERT_EQ(this->alloc.capacity, 0);
    ASSERT_EQ(this->alloc.ptr, nullptr);
}

TYPED_TEST(HeapAllocationTest, AllocateZeroSizeTest)
{
    this->alloc.allocate(0);

    ASSERT_EQ(this->alloc.ptr, nullptr);
    ASSERT_EQ(this->alloc.capacity, 0);
    ASSERT_EQ(this->alloc.ptr, nullptr);

    this->alloc.free();
}

TYPED_TEST(HeapAllocationTest, ReAllocateZeroSizeTest)
{
    this->alloc.allocate(10);
    this->alloc.reallocate(0);

    ASSERT_EQ(this->alloc.ptr, nullptr);
    ASSERT_EQ(this->alloc.capacity, 0);
    ASSERT_EQ(this->alloc.ptr, nullptr);

    this->alloc.free();
}
