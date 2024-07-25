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

#include "core/types/string_view.h"
#include <gtest/gtest.h>

TEST(IgStringViewTest, DefaultConstructorTest)
{
    IgStringView view;
    ASSERT_EQ(view.Size(), 0);
    ASSERT_EQ(view.Data(), nullptr);
}

TEST(IgStringViewTest, CStringConstructorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    ASSERT_EQ(view.Size(), 13);
    ASSERT_EQ(view.Data(), cstr);
}

TEST(IgStringViewTest, CStringWithSizeConstructorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr, 5);
    ASSERT_EQ(view.Size(), 5);
    ASSERT_EQ(view.Data(), cstr);
}

TEST(IgStringViewTest, CopyConstructorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView original(cstr);
    IgStringView copy(original);
    ASSERT_EQ(copy.Size(), original.Size());
    ASSERT_EQ(copy.Data(), original.Data());
}

TEST(IgStringViewTest, MoveConstructorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView original(cstr);
    IgStringView moved(Ignite::Move(original));
    ASSERT_EQ(moved.Size(), 13);
    ASSERT_EQ(moved.Data(), cstr);
    ASSERT_EQ(original.Size(), 0);
    ASSERT_EQ(original.Data(), nullptr);
}

TEST(IgStringViewTest, BeginEndTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    auto begin = view.begin();
    auto end   = view.end();
    ASSERT_EQ(end.Ptr() - begin.Ptr(), 13);
}

TEST(IgStringViewTest, AtTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    ASSERT_EQ(view.At(0), 'H');
    ASSERT_EQ(view.At(12), '!');
}

TEST(IgStringViewTest, FirstLastTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    ASSERT_EQ(view.First(), 'H');
    ASSERT_EQ(view.Last(), '!');
}

TEST(IgStringViewTest, IsEmptyTest)
{
    IgStringView view;
    ASSERT_TRUE(view.IsEmpty());
    const char* cstr = "Hello, World!";
    IgStringView nonEmptyView(cstr);
    ASSERT_FALSE(nonEmptyView.IsEmpty());
}

TEST(IgStringViewTest, IsNullTerminatedTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    ASSERT_TRUE(view.IsNullTerminated());
}

TEST(IgStringViewTest, IndexOperatorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    ASSERT_EQ(view[0], 'H');
    ASSERT_EQ(view[12], '!');
}

TEST(IgStringViewTest, EqualityOperatorTest)
{
    const char* cstr1 = "Hello, World!";
    const char* cstr2 = "Hello, World!";
    IgStringView view1(cstr1);
    IgStringView view2(cstr2);
    ASSERT_TRUE(view1 == view2);
}

TEST(IgStringViewTest, InequalityOperatorTest)
{
    const char* cstr1 = "Hello, World!";
    const char* cstr2 = "Goodbye, World!";
    IgStringView view1(cstr1);
    IgStringView view2(cstr2);
    ASSERT_TRUE(view1 != view2);
}

TEST(IgStringViewTest, AssignmentOperatorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view;
    view = cstr;
    ASSERT_EQ(view.Size(), 13);
    ASSERT_EQ(view.Data(), cstr);
}

TEST(IgStringViewTest, CopyAssignmentOperatorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView original(cstr);
    IgStringView copy;
    copy = original;
    ASSERT_EQ(copy.Size(), original.Size());
    ASSERT_EQ(copy.Data(), original.Data());
}

TEST(IgStringViewTest, MoveAssignmentOperatorTest)
{
    const char* cstr = "Hello, World!";
    IgStringView original(cstr);
    IgStringView moved;
    moved = Ignite::Move(original);
    ASSERT_EQ(moved.Size(), 13);
    ASSERT_EQ(moved.Data(), cstr);
    ASSERT_EQ(original.Size(), 0);
    ASSERT_EQ(original.Data(), nullptr);
}

TEST(IgStringViewTest, SliceTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    IgStringView sliced = view.Slice(7, 5);
    ASSERT_EQ(sliced.Size(), 5);
    ASSERT_EQ(sliced.Data(), cstr + 7);
}

TEST(IgStringViewTest, SetTest)
{
    const char* cstr1 = "Hello, World!";
    const char* cstr2 = "Goodbye, World!";
    IgStringView view1(cstr1);
    IgStringView view2(cstr2);
    view1.Set(view2);
    ASSERT_EQ(view1.Size(), view2.Size());
    ASSERT_EQ(view1.Data(), view2.Data());
}

TEST(IgStringViewTest, CompareTest)
{
    const char* cstr1 = "Hello, World!";
    const char* cstr2 = "Hello, World!";
    IgStringView view1(cstr1);
    IgStringView view2(cstr2);
    ASSERT_TRUE(view1.Compare(view2));
}

TEST(IgStringViewTest, FindTest)
{
    const char* cstr = "Hello, World!";
    IgStringView view(cstr);
    IgStringView match("World");
    ASSERT_EQ(view.Find(match), 7);
}

TEST(IgStringViewTest, FindLastTest)
{
    const char* cstr = "Hello, World! World!";
    IgStringView view(cstr);
    IgStringView match("World");
    ASSERT_EQ(view.FindLast(match), 14);
}

TEST(IgStringViewTest, FindAllTest)
{
    const char* cstr = "Hello, World! World!";
    IgStringView view(cstr);
    IgStringView match("World");
    IgArray<size_t> matches = view.FindAll(match);
    ASSERT_EQ(matches.Size(), 2);
    ASSERT_EQ(matches[0], 7);
    ASSERT_EQ(matches[1], 14);
}
