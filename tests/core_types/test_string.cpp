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

#include "core/strings/string.h"
#include <gtest/gtest.h>

using fiwre::String;

TEST(StringTest, DefaultConstructor) {
  String str;
  EXPECT_EQ(str.size(), 0);
  EXPECT_TRUE(str.is_empty());
}

TEST(StringTest, SizeConstructor) {
  String str(10);
  EXPECT_EQ(str.size(), 10);
  EXPECT_FALSE(str.is_empty());
  EXPECT_EQ(str.data()[10], '\0');
}

TEST(StringTest, StringViewConstructor) {
  fiwre::StringView view("Hello", 5);
  String str(view);
  EXPECT_EQ(str.size(), 5);
  EXPECT_FALSE(str.is_empty());
  EXPECT_STREQ(str.data(), "Hello");
}

TEST(StringTest, CStringConstructor) {
  String str("Hello");
  EXPECT_EQ(str.size(), 5);
  EXPECT_FALSE(str.is_empty());
  EXPECT_STREQ(str.data(), "Hello");
}

TEST(StringTest, CopyConstructor) {
  String str1("Hello");
  String str2(str1);
  EXPECT_EQ(str2.size(), 5);
  EXPECT_FALSE(str2.is_empty());
  EXPECT_STREQ(str2.data(), "Hello");
}

TEST(StringTest, MoveConstructor) {
  String str1("Hello");
  String str2(std::move(str1));
  EXPECT_EQ(str2.size(), 5);
  EXPECT_FALSE(str2.is_empty());
  EXPECT_STREQ(str2.data(), "Hello");
  EXPECT_TRUE(str1.is_empty());
}

TEST(StringTest, AssignmentOperator) {
  String str1("Hello");
  String str2;
  str2 = str1;
  EXPECT_EQ(str2.size(), 5);
  EXPECT_FALSE(str2.is_empty());
  EXPECT_STREQ(str2.data(), "Hello");
}

TEST(StringTest, MoveAssignmentOperator) {
  String str1("Hello");
  String str2;
  str2 = std::move(str1);
  EXPECT_EQ(str2.size(), 5);
  EXPECT_FALSE(str2.is_empty());
  EXPECT_STREQ(str2.data(), "Hello");
  EXPECT_TRUE(str1.is_empty());
}

TEST(StringTest, EqualityOperator) {
  String str1("Hello");
  String str2("Hello");
  EXPECT_TRUE(str1 == str2);
}

TEST(StringTest, InequalityOperator) {
  String str1("Hello");
  String str2("World");
  EXPECT_TRUE(str1 != str2);
}

TEST(StringTest, Append) {
  String str1("Hello");
  String str2(" World");
  str1.append(str2);
  EXPECT_EQ(str1.size(), 11);
  EXPECT_STREQ(str1.data(), "Hello World");
}

TEST(StringTest, Insert) {
  String str1("Hello");
  String str2(" World");
  str1.insert(str1.begin() + 5, str2);
  EXPECT_EQ(str1.size(), 11);
  EXPECT_STREQ(str1.data(), "Hello World");
}

TEST(StringTest, PopBack) {
  String str("Hello");
  str.pop_back();
  EXPECT_EQ(str.size(), 4);
  EXPECT_STREQ(str.data(), "Hell");
}

TEST(StringTest, PopFront) {
  String str("Hello");
  str.pop_front();
  EXPECT_EQ(str.size(), 4);
  EXPECT_STREQ(str.data(), "ello");
}

TEST(StringTest, Erase) {
  String str("Hello");
  str.erase(str.begin() + 1, str.begin() + 3);
  EXPECT_EQ(str.size(), 3);
  EXPECT_STREQ(str.data(), "Hlo");
}

TEST(StringTest, Slice) {
  String str("Hello World");
  fiwre::StringView view = str.slice(0, 5);
  EXPECT_EQ(view.size(), 5);
  EXPECT_TRUE(!view.is_null_terminated());
  EXPECT_TRUE(std::strncmp(view.data(), "Hello", view.size()) == 0);
}

TEST(StringTest, ToUpper) {
  String str("hello");
  str.to_upper();
  EXPECT_STREQ(str.data(), "HELLO");
}

TEST(StringTest, ToLower) {
  String str("HELLO");
  str.ToLower();
  EXPECT_STREQ(str.data(), "hello");
}

TEST(StringTest, Trim) {
  String str("  Hello World  ");
  str.trim();
  EXPECT_STREQ(str.cstr(), "Hello World");
}

TEST(StringTest, Find) {
  String str("Hello World");
  EXPECT_EQ(str.Find("World"), 6);
}

TEST(StringTest, Replace) {
  String str("Hello World. The World is a great place");
  str.replace("World", "Earth");
  EXPECT_STREQ(str.cstr(), "Hello Earth. The Earth is a great place");
}

TEST(StringTest, ReplaceFirst) {
  String str("Hello World. The World is a great place");
  str.replace_first("World", "Earth");
  EXPECT_STREQ(str.cstr(), "Hello Earth. The World is a great place");
}

TEST(StringTest, ReplaceLast) {
  String str("Hello World. The World is a great place");
  str.replace_last("World", "Earth");
  EXPECT_STREQ(str.cstr(), "Hello World. The Earth is a great place");
}
