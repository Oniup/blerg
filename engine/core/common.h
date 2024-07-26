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

#ifndef IGNITE_CORE__COMMON_H
#define IGNITE_CORE__COMMON_H

#include "core/defines.h"
#include "core/memory/allocator.h"
#include <cstdint>
#include <cstdio>

// Allocators
// ------------------------------------------------------------------------------------------------
struct IgHeapAllocation;
template <size_t _Capacity>
struct IgFixedAllocation;

// Iterators
// ------------------------------------------------------------------------------------------------
template <typename _T, typename _PackedContainer>
struct IgPackedIterator;
template <typename _T, typename _PackedContainer>
struct IgConstPackedIterator;

// Strings
// ------------------------------------------------------------------------------------------------
struct IgStringView;
template <typename _T>
struct IgStringViewCast;

template <typename _Allocator                  = IgHeapAllocation,
          size_t _CapacityIncreaseIntervalSize = IG_DEFAULT_CAPACITY_INTERVAL>
struct IgString;

// Common Templated types
// ------------------------------------------------------------------------------------------------
template <typename _T, typename _Allocator = IgHeapAllocation,
          size_t _CapacityIncreaseIntervalSize = IG_DEFAULT_CAPACITY_INTERVAL>
struct IgArray;

#endif
