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

#include "core/strings/string.h"
#include <gtest/gtest.h>

TEST(IgStringTest, DefaultConstructor)
{
    IgString str;
    EXPECT_EQ(str.Size, 0);
    EXPECT_TRUE(str.IsEmpty());
}

TEST(IgStringTest, SizeConstructor)
{
    IgString str(10);
    EXPECT_EQ(str.Size, 10);
    EXPECT_FALSE(str.IsEmpty());
    EXPECT_EQ(str.Data()[10], '\0');
}

TEST(IgStringTest, StringViewConstructor)
{
    IgStringView view("Hello", 5);
    IgString str(view);
    EXPECT_EQ(str.Size, 5);
    EXPECT_FALSE(str.IsEmpty());
    EXPECT_STREQ(str.Data(), "Hello");
}

TEST(IgStringTest, CStringConstructor)
{
    IgString str("Hello");
    EXPECT_EQ(str.Size, 5);
    EXPECT_FALSE(str.IsEmpty());
    EXPECT_STREQ(str.Data(), "Hello");
}

TEST(IgStringTest, CopyConstructor)
{
    IgString str1("Hello");
    IgString str2(str1);
    EXPECT_EQ(str2.Size, 5);
    EXPECT_FALSE(str2.IsEmpty());
    EXPECT_STREQ(str2.Data(), "Hello");
}

TEST(IgStringTest, MoveConstructor)
{
    IgString str1("Hello");
    IgString str2(std::move(str1));
    EXPECT_EQ(str2.Size, 5);
    EXPECT_FALSE(str2.IsEmpty());
    EXPECT_STREQ(str2.Data(), "Hello");
    EXPECT_TRUE(str1.IsEmpty());
}

TEST(IgStringTest, AssignmentOperator)
{
    IgString str1("Hello");
    IgString str2;
    str2 = str1;
    EXPECT_EQ(str2.Size, 5);
    EXPECT_FALSE(str2.IsEmpty());
    EXPECT_STREQ(str2.Data(), "Hello");
}

TEST(IgStringTest, MoveAssignmentOperator)
{
    IgString str1("Hello");
    IgString str2;
    str2 = std::move(str1);
    EXPECT_EQ(str2.Size, 5);
    EXPECT_FALSE(str2.IsEmpty());
    EXPECT_STREQ(str2.Data(), "Hello");
    EXPECT_TRUE(str1.IsEmpty());
}

TEST(IgStringTest, EqualityOperator)
{
    IgString str1("Hello");
    IgString str2("Hello");
    EXPECT_TRUE(str1 == str2);
}

TEST(IgStringTest, InequalityOperator)
{
    IgString str1("Hello");
    IgString str2("World");
    EXPECT_TRUE(str1 != str2);
}

TEST(IgStringTest, Append)
{
    IgString str1("Hello");
    IgString str2(" World");
    str1.Append(str2);
    EXPECT_EQ(str1.Size, 11);
    EXPECT_STREQ(str1.Data(), "Hello World");
}

TEST(IgStringTest, Insert)
{
    IgString str1("Hello");
    IgString str2(" World");
    str1.Insert(str1.Begin() + 5, str2);
    EXPECT_EQ(str1.Size, 11);
    EXPECT_STREQ(str1.Data(), "Hello World");
}

TEST(IgStringTest, PopBack)
{
    IgString str("Hello");
    str.PopBack();
    EXPECT_EQ(str.Size, 4);
    EXPECT_STREQ(str.Data(), "Hell");
}

TEST(IgStringTest, PopFront)
{
    IgString str("Hello");
    str.PopFront();
    EXPECT_EQ(str.Size, 4);
    EXPECT_STREQ(str.Data(), "ello");
}

TEST(IgStringTest, Erase)
{
    IgString str("Hello");
    str.Erase(str.Begin() + 1, str.Begin() + 3);
    EXPECT_EQ(str.Size, 3);
    EXPECT_STREQ(str.Data(), "Hlo");
}

TEST(IgStringTest, Slice)
{
    IgString str("Hello World");
    IgStringView view = str.Slice(0, 5);
    EXPECT_EQ(view.Size, 5);
    EXPECT_TRUE(!view.IsNullTerminated());
    EXPECT_TRUE(std::strncmp(view.Data(), "Hello", view.Size) == 0);
}

TEST(IgStringTest, ToUpper)
{
    IgString str("hello");
    str.ToUpper();
    EXPECT_STREQ(str.Data(), "HELLO");
}

TEST(IgStringTest, ToLower)
{
    IgString str("HELLO");
    str.ToLower();
    EXPECT_STREQ(str.Data(), "hello");
}

TEST(IgStringTest, Trim)
{
    IgString str("  Hello World  ");
    str.Trim();
    EXPECT_STREQ(str.CStr(), "Hello World");
}

TEST(IgStringTest, Find)
{
    IgString str("Hello World");
    EXPECT_EQ(str.Find("World"), 6);
}

TEST(IgStringTest, Replace)
{
    IgString str("Hello World. The World is a great place");
    str.Replace("World", "Earth");
    EXPECT_STREQ(str.CStr(), "Hello Earth. The Earth is a great place");
}

TEST(IgStringTest, ReplaceFirst)
{
    IgString str("Hello World. The World is a great place");
    str.ReplaceFirst("World", "Earth");
    EXPECT_STREQ(str.CStr(), "Hello Earth. The World is a great place");
}

TEST(IgStringTest, ReplaceLast)
{
    IgString str("Hello World. The World is a great place");
    str.ReplaceLast("World", "Earth");
    EXPECT_STREQ(str.CStr(), "Hello World. The Earth is a great place");
}
