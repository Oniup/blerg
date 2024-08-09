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

#include "core/containers/array.h"
#include <gtest/gtest.h>

using fiwre::Array;

TEST(ArrayTest, DefaultConstructorTest) {
  Array<int> array;
  ASSERT_EQ(array.size(), 0);
  ASSERT_TRUE(array.is_empty());
}

TEST(ArrayTest, SizeConstructorTest) {
  Array<int> array(10);
  ASSERT_EQ(array.size(), 10);
}

TEST(ArrayTest, InitializerListConstructorTest) {
  Array<int> array({1, 2, 3, 4, 5});
  ASSERT_EQ(array.size(), 5);
  for (size_t i = 0; i < array.size(); ++i) {
    ASSERT_EQ(array[i], i + 1);
  }
}

TEST(ArrayTest, CopyConstructorTest) {
  Array<int> array({1, 2, 3, 4, 5});
  Array<int> copy(array);
  ASSERT_EQ(copy.size(), array.size());
  for (size_t i = 0; i < copy.size(); ++i) {
    ASSERT_EQ(copy[i], array[i]);
  }
}

TEST(ArrayTest, MoveConstructorTest) {
  Array<int> array({1, 2, 3, 4, 5});
  Array<int> moved(fiwre::move(array));
  ASSERT_EQ(moved.size(), 5);
  for (size_t i = 0; i < moved.size(); ++i) {
    ASSERT_EQ(moved[i], i + 1);
  }
  ASSERT_EQ(array.size(), 0);
}

TEST(ArrayTest, CopyAssignmentTest) {
  Array<int> array({1, 2, 3, 4, 5});
  Array<int> copy;
  copy = array;
  ASSERT_EQ(copy.size(), array.size());
  for (size_t i = 0; i < copy.size(); ++i) {
    ASSERT_EQ(copy[i], array[i]);
  }
}

TEST(ArrayTest, MoveAssignmentTest) {
  Array<int> array({1, 2, 3, 4, 5});
  Array<int> moved;
  moved = fiwre::move(array);
  ASSERT_EQ(moved.size(), 5);
  for (size_t i = 0; i < moved.size(); ++i) {
    ASSERT_EQ(moved[i], i + 1);
  }
  ASSERT_EQ(array.size(), 0);
}

TEST(ArrayTest, EqualityTest) {
  Array<int> array({1, 2, 3, 4, 5});
  Array<int> copy(array);
  ASSERT_TRUE(array == copy);
}

TEST(ArrayTest, InequalityTest) {
  Array<int> array({1, 2, 3, 4, 5});
  Array<int> different({1, 2, 3, 4, 6});
  ASSERT_TRUE(array != different);
}

TEST(ArrayTest, IndexOperatorTest) {
  Array<int> array({1, 2, 3, 4, 5});
  ASSERT_EQ(array[0], 1);
  ASSERT_EQ(array[4], 5);
}

TEST(ArrayTest, ClearTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.clear();
  ASSERT_EQ(array.size(), 0);
}

TEST(ArrayTest, DestroyTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.~Array();
  // Since the destructor calls Free, which sets Size to 0, we can check that
  // here
  ASSERT_EQ(array.size(), 0);
}

TEST(ArrayTest, ResizeTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.resize(3);
  ASSERT_EQ(array.size(), 3);
}

TEST(ArrayTest, ReserveToFitTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.reserve_to_fit(10);
  ASSERT_GE(array.capacity(), 10);
}

TEST(ArrayTest, ReserveTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.reserve(10);
  ASSERT_EQ(array.capacity(), 10);
}

TEST(ArrayTest, PushBackTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.push_back(6);
  ASSERT_EQ(array.size(), 6);
  ASSERT_EQ(array[5], 6);
}

TEST(ArrayTest, PushFrontTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.push_front(0);
  ASSERT_EQ(array.size(), 6);
  ASSERT_EQ(array[0], 0);
}

TEST(ArrayTest, InsertTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.insert(array.begin() + 2, 99);
  ASSERT_EQ(array.size(), 6);
  ASSERT_EQ(array[2], 99);
}

TEST(ArrayTest, PopBackTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.pop_back();
  ASSERT_EQ(array.size(), 4);
  ASSERT_EQ(array[3], 4);
}

TEST(ArrayTest, PopFrontTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.pop_front();
  ASSERT_EQ(array.size(), 4);
  ASSERT_EQ(array[0], 2);
}

TEST(ArrayTest, EraseTest) {
  Array<int> array({1, 2, 3, 4, 5});
  array.erase(array.begin() + 2, array.begin() + 4);
  ASSERT_EQ(array.size(), 3);
  ASSERT_EQ(array[0], 1);
  ASSERT_EQ(array[1], 2);
  ASSERT_EQ(array[2], 5);
}

TEST(ArrayTest, FindTest) {
  Array<int> array({1, 2, 3, 4, 5});
  ASSERT_EQ(array.find(3), 2);
  ASSERT_EQ(array.find(10), Array<int>::NoPos);
}

TEST(ArrayTest, FindLastTest) {
  Array<int> array({1, 2, 3, 4, 5});
  ASSERT_EQ(array.find_last(3), 2);
  ASSERT_EQ(array.find_last(10), Array<int>::NoPos);
}
