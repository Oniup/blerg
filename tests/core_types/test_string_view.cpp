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

#include "core/strings/string_view.h"
#include <gtest/gtest.h>

using fiwre::StringView;

TEST(StringViewTest, DefaultConstructorTest) {
  StringView view;
  ASSERT_EQ(view.size(), 0);
  ASSERT_EQ(view.data(), nullptr);
}

TEST(StringViewTest, CStringConstructorTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  ASSERT_EQ(view.size(), 13);
  ASSERT_EQ(view.data(), cstr);
}

TEST(StringViewTest, CStringWithSizeConstructorTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr, 5);
  ASSERT_EQ(view.size(), 5);
  ASSERT_EQ(view.data(), cstr);
}

TEST(StringViewTest, CopyConstructorTest) {
  const char* cstr = "Hello, World!";
  StringView original(cstr);
  StringView copy(original);
  ASSERT_EQ(copy.size(), original.size());
  ASSERT_EQ(copy.data(), original.data());
}

TEST(StringViewTest, MoveConstructorTest) {
  const char* cstr = "Hello, World!";
  StringView original(cstr);
  StringView moved(move(original));
  ASSERT_EQ(moved.size(), 13);
  ASSERT_EQ(moved.data(), cstr);
  ASSERT_EQ(original.size(), 0);
  ASSERT_EQ(original.data(), nullptr);
}

TEST(StringViewTest, BeginEndTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  auto begin = view.begin();
  auto end   = view.end();
  ASSERT_EQ(end.ptr() - begin.ptr(), 13);
}

TEST(StringViewTest, AtTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  ASSERT_EQ(view.at(0), 'H');
  ASSERT_EQ(view.at(12), '!');
}

TEST(StringViewTest, FirstLastTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  ASSERT_EQ(view.first(), 'H');
  ASSERT_EQ(view.last(), '!');
}

TEST(StringViewTest, IsEmptyTest) {
  StringView view;
  ASSERT_TRUE(view.is_empty());
  const char* cstr = "Hello, World!";
  StringView nonEmptyView(cstr);
  ASSERT_FALSE(nonEmptyView.is_empty());
}

TEST(StringViewTest, IsNullTerminatedTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  ASSERT_TRUE(view.is_null_terminated());
}

TEST(StringViewTest, IndexOperatorTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  ASSERT_EQ(view[0], 'H');
  ASSERT_EQ(view[12], '!');
}

TEST(StringViewTest, EqualityOperatorTest) {
  const char* cstr1 = "Hello, World!";
  const char* cstr2 = "Hello, World!";
  StringView view1(cstr1);
  StringView view2(cstr2);
  ASSERT_TRUE(view1 == view2);
}

TEST(StringViewTest, InequalityOperatorTest) {
  const char* cstr1 = "Hello, World!";
  const char* cstr2 = "Goodbye, World!";
  StringView view1(cstr1);
  StringView view2(cstr2);
  ASSERT_TRUE(view1 != view2);
}

TEST(StringViewTest, AssignmentOperatorTest) {
  const char* cstr = "Hello, World!";
  StringView view;
  view = cstr;
  ASSERT_EQ(view.size(), 13);
  ASSERT_EQ(view.data(), cstr);
}

TEST(StringViewTest, CopyAssignmentOperatorTest) {
  const char* cstr = "Hello, World!";
  StringView original(cstr);
  StringView copy;
  copy = original;
  ASSERT_EQ(copy.size(), original.size());
  ASSERT_EQ(copy.data(), original.data());
}

TEST(StringViewTest, MoveAssignmentOperatorTest) {
  const char* cstr = "Hello, World!";
  StringView original(cstr);
  StringView moved;
  moved = move(original);
  ASSERT_EQ(moved.size(), 13);
  ASSERT_EQ(moved.data(), cstr);
  ASSERT_EQ(original.size(), 0);
  ASSERT_EQ(original.data(), nullptr);
}

TEST(StringViewTest, SliceTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  StringView sliced = view.slice(7, 5);
  ASSERT_EQ(sliced.size(), 5);
  ASSERT_EQ(sliced.data(), cstr + 7);
}

TEST(StringViewTest, SetTest) {
  const char* cstr1 = "Hello, World!";
  const char* cstr2 = "Goodbye, World!";
  StringView view1(cstr1);
  StringView view2(cstr2);
  view1.set(view2);
  ASSERT_EQ(view1.size(), view2.size());
  ASSERT_EQ(view1.data(), view2.data());
}

TEST(StringViewTest, CompareTest) {
  const char* cstr1 = "Hello, World!";
  const char* cstr2 = "Hello, World!";
  StringView view1(cstr1);
  StringView view2(cstr2);
  ASSERT_TRUE(view1.compare(view2));
}

TEST(StringViewTest, FindTest) {
  const char* cstr = "Hello, World!";
  StringView view(cstr);
  StringView match("World");
  ASSERT_EQ(view.find(match), 7);
}

TEST(StringViewTest, FindLastTest) {
  const char* cstr = "Hello, World! World!";
  StringView view(cstr);
  StringView match("World");
  ASSERT_EQ(view.find_last(match), 14);
}

TEST(StringViewTest, FindAllTest) {
  const char* cstr = "Hello, World! World!";
  StringView view(cstr);
  StringView match("World");
  fiwre::Array<size_t> matches;
  view.find_all(matches, match);
  ASSERT_EQ(matches.size(), 2);
  ASSERT_EQ(matches[0], 7);
  ASSERT_EQ(matches[1], 14);
}
