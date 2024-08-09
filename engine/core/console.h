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

#ifndef CORE__CONSOLE_H
#define CORE__CONSOLE_H

#include "containers/array.h"
#include <stdint.h>

namespace fiwre::util {

typedef uint32_t ConsoleSeverityFlags;
typedef uint32_t ConsoleOutputStyleFlags;

enum ConsoleSeverity {
  ConsoleSeverity_Invalid = 0x0000,
  ConsoleSeverity_Verbose = 0x0001,
  ConsoleSeverity_Trace   = 0x0002,
  ConsoleSeverity_Info    = 0x0004,
  ConsoleSeverity_Warning = 0x0008,
  ConsoleSeverity_Error   = 0x0010,
  ConsoleSeverity_Fatal   = 0x0020,
};

enum ConsoleOutputStyle {
  ConsoleOutputStyle_None             = 0x0000,
  ConsoleOutputStyle_Color            = 0x0001,
  ConsoleOutputStyle_FlushPerMessage  = 0x0002,
  ConsoleOutputStyle_ExcludeFile      = 0x0004,
  ConsoleOutputStyle_ExcludeLine      = 0x0008,
  ConsoleOutputStyle_ExcludeFunction  = 0x0010,
  ConsoleOutputStyle_BreakAfterHeader = 0x0020,
  ConsoleOutputStyle_BreakAfterInfo   = 0x0040,
};

struct ConsoleMsgData {};

struct ConsoleOutput {
  ConsoleOutputStyleFlags style = ConsoleOutputStyle_None;
  ConsoleSeverityFlags severity = ConsoleSeverity_Info |
                                  ConsoleSeverity_Warning |
                                  ConsoleSeverity_Error | ConsoleSeverity_Fatal;

  virtual ~ConsoleOutput() = default;

  virtual const char* name() const                     = 0;
  virtual void print_output(const ConsoleMsgData& msg) = 0;

protected:
  virtual char* _format_head(const ConsoleMsgData& msg);
  virtual char* _format_body(const ConsoleMsgData& msg);
};

class ConsoleManager {
public:
  static ConsoleManager create();

  void destroy();

private:
  Array<ConsoleOutput*> outputs;
};

} // namespace fiwre::util

#endif
