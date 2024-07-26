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

#include "core/memory/allocator.h"
#include <gtest/gtest.h>

template <typename _T>
class IgHeapAllocationTest : public ::testing::Test {
protected:
    typename IgHeapAllocation::template Allocation<_T> alloc;
};

using TestTypes = testing::Types<char, int, double>;
TYPED_TEST_CASE(IgHeapAllocationTest, TestTypes);

TYPED_TEST(IgHeapAllocationTest, AllocateTest)
{
    this->alloc.Allocate(10);

    ASSERT_NE(this->alloc.Ptr, nullptr);
    ASSERT_EQ(this->alloc.Capacity, 10);

    this->alloc.Free();
    ASSERT_EQ(this->alloc.Capacity, 0);
    ASSERT_EQ(this->alloc.Ptr, nullptr);
}

TYPED_TEST(IgHeapAllocationTest, ReAllocateTest)
{
    this->alloc.Allocate(10);
    this->alloc.ReAllocate(20);

    ASSERT_NE(this->alloc.Ptr, nullptr);
    ASSERT_EQ(this->alloc.Capacity, 20);

    this->alloc.Free();
    ASSERT_EQ(this->alloc.Capacity, 0);
    ASSERT_EQ(this->alloc.Ptr, nullptr);
}

TYPED_TEST(IgHeapAllocationTest, ReAllocateWithoutInitialAllocationTest)
{
    this->alloc.ReAllocate(20);

    ASSERT_NE(this->alloc.Ptr, nullptr);
    ASSERT_EQ(this->alloc.Capacity, 20);

    this->alloc.Free();
    ASSERT_EQ(this->alloc.Capacity, 0);
    ASSERT_EQ(this->alloc.Ptr, nullptr);
}

TYPED_TEST(IgHeapAllocationTest, AllocateZeroSizeTest)
{
    this->alloc.Allocate(0);

    ASSERT_EQ(this->alloc.Ptr, nullptr);
    ASSERT_EQ(this->alloc.Capacity, 0);
    ASSERT_EQ(this->alloc.Ptr, nullptr);

    this->alloc.Free();
}

TYPED_TEST(IgHeapAllocationTest, ReAllocateZeroSizeTest)
{
    this->alloc.Allocate(10);
    this->alloc.ReAllocate(0);

    ASSERT_EQ(this->alloc.Ptr, nullptr);
    ASSERT_EQ(this->alloc.Capacity, 0);
    ASSERT_EQ(this->alloc.Ptr, nullptr);

    this->alloc.Free();
}
