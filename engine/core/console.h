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

#ifndef IGNITE_CORE__CONSOLE_H
#define IGNITE_CORE__CONSOLE_H

#ifdef _MSC_VER
#    define _IG_GENERATE_TRAP() __debugbreak()
#else
#    define _Ignite_GENERATE_TRAP() __builtin_trap()
#endif

#define Ignite_FUNCTION_STR __FUNCTION__

#include <stdint.h>

typedef uint32_t IgConsoleSeverityFlags;
typedef uint32_t IgConsoleOutputStyleFlags;

enum IgConsoleSeverity {
    IgConsoleSeverity_Invalid = 0x0000,
    IgConsoleSeverity_Verbose = 0x0001,
    IgConsoleSeverity_Trace   = 0x0002,
    IgConsoleSeverity_Info    = 0x0004,
    IgConsoleSeverity_Warning = 0x0008,
    IgConsoleSeverity_Error   = 0x0010,
    IgConsoleSeverity_Fatal   = 0x0020,
};

enum IgConsoleOutputStyle {
    IgConsoleOutputStyle_None             = 0x0000,
    IgConsoleOutputStyle_Color            = 0x0001,
    IgConsoleOutputStyle_FlushPerMessage  = 0x0002,
    IgConsoleOutputStyle_ExcludeFile      = 0x0004,
    IgConsoleOutputStyle_ExcludeLine      = 0x0008,
    IgConsoleOutputStyle_ExcludeFunction  = 0x0010,
    IgConsoleOutputStyle_BreakAfterHeader = 0x0020,
    IgConsoleOutputStyle_BreakAfterInfo   = 0x0040,
};

struct IgConsoleMsgData {};

struct IgConsoleOutputBase {
    IgConsoleOutputStyleFlags Style = IgConsoleOutputStyle_None;
    IgConsoleSeverityFlags Severity = IgConsoleSeverity_Info | IgConsoleSeverity_Warning |
                                      IgConsoleSeverity_Error | IgConsoleSeverity_Fatal;

    virtual ~IgConsoleOutputBase() = default;

    virtual const char* name() const                       = 0;
    virtual void print_output(const IgConsoleMsgData& msg) = 0;

protected:
    virtual char* _format_head(const IgConsoleMsgData& msg);
    virtual char* _format_body(const IgConsoleMsgData& msg);
};

struct IgConsoleManager {};

namespace Ignite {

void InitConsoleManager();
void DestroyConsoleManager();

} // namespace Ignite

#endif
