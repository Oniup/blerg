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

#ifndef CORE_CONSOLE_H
#define CORE_CONSOLE_H

#include "core/defines.h"
#include <fmt/format.h>
#include <string>
#include <string_view>
#include <vector>

#define INTERNAL_MSG(_context, _severity, ...)                                 \
  Console::print_to_outputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__,      \
                            __FUNCTION__, (_context),                          \
                            ConsoleOutput_Severity##_severity##Bit)

#define INTERNAL_MSG_RETURN(_context, _returning, _severity, ...)              \
  Console::print_to_outputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__,      \
                            __FUNCTION__, (_context),                          \
                            ConsoleOutput_Severity##_severity##Bit);           \
  return (_returning)

#ifndef NDEBUG
#define INTERNAL_FATAL_MSG(_context, _severity, ...)                           \
  Console::print_to_outputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__,      \
                            __FUNCTION__, (_context),                          \
                            ConsoleOutput_Severity##_severity##Bit);           \
  GENERATE_TRAP()
#else
#define INTERNAL_FATAL_MSG(_context, _severity, ...)
#endif

#define INTERNAL_CONDITION(_context, _condition, _severity, ...)               \
  if (!(_condition)) {                                                         \
    Console::print_to_outputs(                                                 \
        __LINE__,                                                              \
        fmt::format(fmt::format("`{}` == FALSE: ", #_condition) +              \
                    __VA_ARGS__),                                              \
        __FILE__, __FUNCTION__, (_context),                                    \
        ConsoleOutput_Severity##_severity##Bit);                               \
    return;                                                                    \
  } else                                                                       \
    ((void)0)

#define INTERNAL_CONDITION_RETURN(_context, _returning, _condition, _severity, \
                                  ...)                                         \
  if (!(_condition)) {                                                         \
    Console::print_to_outputs(                                                 \
        __LINE__,                                                              \
        fmt::format(fmt::format("`{}` == FALSE: ", #_condition) +              \
                    __VA_ARGS__),                                              \
        __FILE__, __FUNCTION__, (_context),                                    \
        ConsoleOutput_Severity##_severity##Bit);                               \
    return (_returning);                                                       \
  } else                                                                       \
    ((void)0)

#ifndef NDEBUG
#define INTERNAL_FATAL_CONDITION(_context, _condition, _severity, ...)         \
  if (!(_condition)) {                                                         \
    Console::print_to_outputs(                                                 \
        __LINE__,                                                              \
        fmt::format(fmt::format("`{}` == FALSE: ", #_condition) +              \
                    __VA_ARGS__),                                              \
        __FILE__, __FUNCTION__, (_context),                                    \
        ConsoleOutput_Severity##_severity##Bit);                               \
    GENERATE_TRAP();                                                           \
  } else                                                                       \
    ((void)0)
#else
#define INTERNAL_FATAL_CONDITION(_context, _condition, _severity, ...)         \
  (_condition)
#endif

// Basic
// ------------------------------------------------------------------------------------------------
#define VERBOSE(...) INTERNAL_MSG(nullptr, Verbose, __VA_ARGS__)
#define TRACE(...) INTERNAL_MSG(nullptr, Trace, __VA_ARGS__)
#define INFO(...) INTERNAL_MSG(nullptr, Info, __VA_ARGS__)
#define WARN(...) INTERNAL_MSG(nullptr, Warning, __VA_ARGS__)
#define ERROR(...) INTERNAL_MSG(nullptr, Error, __VA_ARGS__)
#define FATAL(...) INTERNAL_FATAL_MSG(nullptr, Fatal, __VA_ARGS__)

#define CONTEXT_VERBOSE(_context, ...)                                         \
  INTERNAL_MSG(_context, Verbose, __VA_ARGS__)
#define CONTEXT_TRACE(_context, ...) INTERNAL_MSG(_context, Trace, __VA_ARGS__)
#define CONTEXT_INFO(_context, ...) INTERNAL_MSG(_context, Info, __VA_ARGS__)
#define CONTEXT_WARN(_context, ...) INTERNAL_MSG(_context, Warning, __VA_ARGS__)
#define CONTEXT_ERROR(_context, ...) INTERNAL_MSG(_context, Error, __VA_ARGS__)
#define CONTEXT_FATAL(_context, ...)                                           \
  INTERNAL_FATAL_MSG(_context, Fatal, __VA_ARGS__)

// Return Value
// ------------------------------------------------------------------------------------------------
#define WARN_RETURN(_returning, ...)                                           \
  INTERNAL_MSG_RETURN(nullptr, _returning, Warning, __VA_ARGS__)
#define ERROR_RETURN(_returning, ...)                                          \
  INTERNAL_MSG_RETURN(nullptr, _returning, Error, __VA_ARGS__)

#define CONTEXT_WARN_RETURN(_context, _returning, ...)                         \
  INTERNAL_MSG_RETURN(_context, _returning, Warning, __VA_ARGS__)
#define CONTEXT_ERROR_RETURN(_context, _returning, ...)                        \
  INTERNAL_MSG_RETURN(_context, _returning, Error, __VA_ARGS__)

// Conditions
// ------------------------------------------------------------------------------------------------
#define CONDITION_WARN(_condition, ...)                                        \
  INTERNAL_CONDITION(nullptr, _condition, Warning, __VA_ARGS__);
#define CONDITION_ERROR(_condition, ...)                                       \
  INTERNAL_CONDITION(nullptr, _condition, Error, __VA_ARGS__);
#define CONDITION_FATAL(_condition, ...)                                       \
  INTERNAL_FATAL_CONDITION(nullptr, _condition, Fatal, __VA_ARGS__);

#define CONDITION_WARN_RETURN(_condition, _returning, ...)                     \
  INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, Warning,          \
                            __VA_ARGS__);
#define CONDITION_ERROR_RETURN(_condition, _returning, ...)                    \
  INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, Error,            \
                            __VA_ARGS__);

// Context
// ------------------------------------------------------------------------------------------------
#define CONTEXT_CONDITION_WARN(_context, _condition, ...)                      \
  INTERNAL_CONDITION(_context, _condition, Warning, __VA_ARGS__);
#define CONTEXT_CONDITION_ERROR(_context, _condition, ...)                     \
  INTERNAL_CONDITION(_context, _condition, Error, __VA_ARGS__);
#define CONTEXT_CONDITION_FATAL(_context, _condition, ...)                     \
  INTERNAL_FATAL_CONDITION(_context, _condition, Fatal, __VA_ARGS__);

#define CONTEXT_CONDITION_WARN_RETURN(_context, _condition, _returning, ...)   \
  INTERNAL_CONDITION_RETURN(_context, _returning, _condition, Warning,         \
                            __VA_ARGS__);
#define CONTEXT_CONDITION_ERROR_RETURN(_context, _condition, _returning, ...)  \
  INTERNAL_CONDITION_RETURN(_context, _returning, _condition, Error,           \
                            __VA_ARGS__);

// Render Hardware Interface
// ------------------------------------------------------------------------------------------------
#ifdef RHI_USE_OPENGL

#define RHI_VERBOSE(...) CONTEXT_VERBOSE("OPENGL", __VA_ARGS__)
#define RHI_TRACE(...) CONTEXT_TRACE("OPENGL", __VA_ARGS__)
#define RHI_INFO(...) CONTEXT_INFO("OPENGL", __VA_ARGS__)
#define RHI_WARN(...) CONTEXT_WARN("OPENGL", __VA_ARGS__)
#define RHI_ERROR(...) CONTEXT_ERROR("OPENGL", __VA_ARGS__)
#define RHI_FATAL(...) CONTEXT_FATAL("OPENGL", __VA_ARGS__)

#define RHI_WARN_RETURN(_returning, ...)                                       \
  CONTEXT_WARN_RETURN("OPENGL", _returning, __VA_ARGS__)
#define RHI_ERROR_RETURN(_returning, ...)                                      \
  CONTEXT_ERROR_RETURN("OPENGL", _returning, __VA_ARGS__)
#define RHI_FATAL_RETURN(_returning, ...)                                      \
  CONTEXT_ERROR_RETURN("OPENGL", _returning, __VA_ARGS__)

#define RHI_CONDITION_WARN(_condition, ...)                                    \
  CONTEXT_CONDITION_WARN("OPENGL", _condition, __VA_ARGS__)
#define RHI_CONDITION_ERROR(_condition, ...)                                   \
  CONTEXT_CONDITION_ERROR("OPENGL", _condition, __VA_ARGS__)
#define RHI_CONDITION_FATAL(_condition, ...)                                   \
  CONTEXT_CONDITION_FATAL("OPENGL", _condition, __VA_ARGS__)

#define RHI_CONDITION_WARN_RETURN(_condition, _returning, ...)                 \
  CONTEXT_CONDITION_WARN_RETURN("OPENGL", _condition, _returning, __VA_ARGS__)
#define RHI_CONDITION_ERROR_RETURN(_condition, _returning, ...)                \
  CONTEXT_CONDITION_ERROR_RETURN("OPENGL", _condition, _returning, __VA_ARGS__)

#elif RHI_USE_VULKAN

#define RHI_VERBOSE(...) CONTEXT_VERBOSE("VULKAN", __VA_ARGS__)
#define RHI_TRACE(...) CONTEXT_TRACE("VULKAN", __VA_ARGS__)
#define RHI_INFO(...) CONTEXT_INFO("VULKAN", __VA_ARGS__)
#define RHI_WARN(...) CONTEXT_WARN("VULKAN", __VA_ARGS__)
#define RHI_ERROR(...) CONTEXT_ERROR("VULKAN", __VA_ARGS__)
#define RHI_FATAL(...) CONTEXT_FATAL("VULKAN", __VA_ARGS__)

#define RHI_WARN_RETURN(_returning, ...)                                       \
  CONTEXT_WARN_RETURN("VULKAN", _returning, __VA_ARGS__)
#define RHI_ERROR_RETURN(_returning, ...)                                      \
  CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)
#define RHI_FATAL_RETURN(_returning, ...)                                      \
  CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)

#define RHI_CONDITION_WARN(_condition, ...)                                    \
  CONTEXT_CONDITION_WARN("VULKAN", _condition, __VA_ARGS__)
#define RHI_CONDITION_ERROR(_condition, ...)                                   \
  CONTEXT_CONDITION_ERROR("VULKAN", _condition, __VA_ARGS__)
#define RHI_CONDITION_FATAL(_condition, ...)                                   \
  CONTEXT_CONDITION_FATAL("VULKAN", _condition, __VA_ARGS__)

#define RHI_CONDITION_WARN_RETURN(_condition, _returning, ...)                 \
  CONTEXT_CONDITION_WARN_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)
#define RHI_CONDITION_ERROR_RETURN(_condition, _returning, ...)                \
  CONTEXT_CONDITION_ERROR_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)

#endif

struct ConsoleMessage;

enum ConsoleOutputFlag {
  ConsoleOutput_NoneBit             = 0,
  ConsoleOutput_FlushPerMessageBit  = 1 << 0,
  ConsoleOutput_ColorBit            = 1 << 1,
  ConsoleOutput_BreakAfterHeaderBit = 1 << 2,
  ConsoleOutput_BreakAfterInfoBit   = 1 << 3,
  ConsoleOutput_FilterFileBit       = 1 << 4,
  ConsoleOutput_FilterLineBit       = 1 << 5,
  ConsoleOutput_FilterFunctionBit   = 1 << 6,

  ConsoleOutput_SeverityVerboseBit = 1 << 7,
  ConsoleOutput_SeverityTraceBit   = 1 << 8,
  ConsoleOutput_SeverityInfoBit    = 1 << 9,
  ConsoleOutput_SeverityWarnBit    = 1 << 10,
  ConsoleOutput_SeverityErrorBit   = 1 << 11,
  ConsoleOutput_SeverityFatalBit   = 1 << 12,
};

const std::string_view console_severity_to_str(int severity);

class ConsoleOutput {
public:
  static constexpr int s_DefaultOptions =
      ConsoleOutput_FlushPerMessageBit | ConsoleOutput_ColorBit |
      ConsoleOutput_BreakAfterInfoBit | ConsoleOutput_BreakAfterHeaderBit;
  static constexpr int s_DefaultSeverity = ConsoleOutput_SeverityWarnBit |
                                           ConsoleOutput_SeverityErrorBit |
                                           ConsoleOutput_SeverityFatalBit;

public:
  ConsoleOutput(int opts = s_DefaultOptions)
        : _opts(opts) {}

  inline int opts() const { return _opts; }

  virtual ~ConsoleOutput() {}

  virtual std::string_view name() const                = 0;
  virtual void print_output(const ConsoleMessage& msg) = 0;

  virtual std::string _format_head(const ConsoleMessage& msg);
  virtual std::string _format_body(const ConsoleMessage& msg);

protected:
  int _opts;
};

class ConsoleTerminalOutput : public ConsoleOutput {
public:
  ConsoleTerminalOutput(int opts = ConsoleOutput::s_DefaultOptions |
                                   ConsoleOutput::s_DefaultSeverity);
  ~ConsoleTerminalOutput() override {};

  inline std::string_view name() const override {
    return "Error Terminal Output";
  }
  void print_output(const ConsoleMessage& msg) override;
};

class Console {
public:
  static void print_to_outputs(int line, const std::string& msg,
                               const char* file, const char* fn,
                               const char* ctx, int severity);

public:
  Console();

  void destroy();

  void add_output(ConsoleOutput* output);

  template <typename TConsoleOutput>
  void add_output(uint32_t opts = ConsoleOutput::s_DefaultOptions |
                                  ConsoleOutput::s_DefaultSeverity) {
    add_output(new TConsoleOutput(opts));
  }

private:
  std::vector<ConsoleOutput*> _outputs;
};

#endif
