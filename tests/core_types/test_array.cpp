// This file is part of Ignite Engine (https://github.com/Oniup/ignite_engine)
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

#include "core/types/array.h"
#include <gtest/gtest.h>

TEST(IgArrayTest, DefaultConstructorTest)
{
    IgArray<int> array;
    ASSERT_EQ(array.Size, 0);
}

TEST(IgArrayTest, SizeConstructorTest)
{
    IgArray<int> array(10);
    ASSERT_EQ(array.Size, 10);
}

TEST(IgArrayTest, InitializerListConstructorTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    ASSERT_EQ(array.Size, 5);
    for (size_t i = 0; i < array.Size; ++i) {
        ASSERT_EQ(array[i], i + 1);
    }
}

TEST(IgArrayTest, CopyConstructorTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    IgArray<int> copy(array);
    ASSERT_EQ(copy.Size, array.Size);
    for (size_t i = 0; i < copy.Size; ++i) {
        ASSERT_EQ(copy[i], array[i]);
    }
}

TEST(IgArrayTest, MoveConstructorTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    IgArray<int> moved(Ignite::Move(array));
    ASSERT_EQ(moved.Size, 5);
    for (size_t i = 0; i < moved.Size; ++i) {
        ASSERT_EQ(moved[i], i + 1);
    }
    ASSERT_EQ(array.Size, 0);
}

TEST(IgArrayTest, CopyAssignmentTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    IgArray<int> copy;
    copy = array;
    ASSERT_EQ(copy.Size, array.Size);
    for (size_t i = 0; i < copy.Size; ++i) {
        ASSERT_EQ(copy[i], array[i]);
    }
}

TEST(IgArrayTest, MoveAssignmentTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    IgArray<int> moved;
    moved = Ignite::Move(array);
    ASSERT_EQ(moved.Size, 5);
    for (size_t i = 0; i < moved.Size; ++i) {
        ASSERT_EQ(moved[i], i + 1);
    }
    ASSERT_EQ(array.Size, 0);
}

TEST(IgArrayTest, EqualityTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    IgArray<int> copy(array);
    ASSERT_TRUE(array == copy);
}

TEST(IgArrayTest, InequalityTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    IgArray<int> different({1, 2, 3, 4, 6});
    ASSERT_TRUE(array != different);
}

TEST(IgArrayTest, IndexOperatorTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[4], 5);
}

TEST(IgArrayTest, ClearTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.Clear();
    ASSERT_EQ(array.Size, 0);
}

TEST(IgArrayTest, DestroyTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.~IgArray();
    // Since the destructor calls Free, which sets Size to 0, we can check that here
    ASSERT_EQ(array.Size, 0);
}

TEST(IgArrayTest, ResizeTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.Resize(3);
    ASSERT_EQ(array.Size, 3);
}

TEST(IgArrayTest, ReserveToFitTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.ReserveToFit(10);
    ASSERT_GE(array.Capacity(), 10);
}

TEST(IgArrayTest, ReserveTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.Reserve(10);
    ASSERT_EQ(array.Capacity(), 10);
}

TEST(IgArrayTest, PushBackTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.PushBack(6);
    ASSERT_EQ(array.Size, 6);
    ASSERT_EQ(array[5], 6);
}

TEST(IgArrayTest, PushFrontTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.PushFront(0);
    ASSERT_EQ(array.Size, 6);
    ASSERT_EQ(array[0], 0);
}

TEST(IgArrayTest, InsertTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.Insert(array.Begin() + 2, 99);
    ASSERT_EQ(array.Size, 6);
    ASSERT_EQ(array[2], 99);
}

TEST(IgArrayTest, PopBackTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.PopBack();
    ASSERT_EQ(array.Size, 4);
    ASSERT_EQ(array[3], 4);
}

TEST(IgArrayTest, PopFrontTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.PopFront();
    ASSERT_EQ(array.Size, 4);
    ASSERT_EQ(array[0], 2);
}

TEST(IgArrayTest, EraseTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    array.Erase(array.Begin() + 2, array.Begin() + 4);
    ASSERT_EQ(array.Size, 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 5);
}

TEST(IgArrayTest, FindTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    ASSERT_EQ(array.Find(3), 2);
    ASSERT_EQ(array.Find(10), IgArray<int>::NoPos);
}

TEST(IgArrayTest, FindLastTest)
{
    IgArray<int> array({1, 2, 3, 4, 5});
    ASSERT_EQ(array.FindLast(3), 2);
    ASSERT_EQ(array.FindLast(10), IgArray<int>::NoPos);
}
