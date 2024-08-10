// This file is part of Fiwre (https://github.com/oniup/fiwre)
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

#include "core/console.h"
#include <fmt/color.h>

struct ConsoleMessage {
  int line = -1;
  std::string msg;
  const char* file;
  const char* fn;
  const char* ctx;
  int severity;
};

static Console* s_ptr = nullptr;

const std::string_view console_severity_to_str(ConsoleOutputFlag severity) {
  switch (severity) {
    case ConsoleOutput_SeverityTrace:
      return "Trace";
    case ConsoleOutput_SeverityVerbose:
      return "Verbose";
    case ConsoleOutput_SeverityInfo:
      return "Info";
    case ConsoleOutput_SeverityWarning:
      return "Warning";
    case ConsoleOutput_SeverityError:
      return "Error";
    case ConsoleOutput_SeverityFatal:
      return "Fatal";
    default:
      return "Invalid";
  }
}

std::string ConsoleOutput::_format_head(const ConsoleMessage& msg) {
  fmt::text_style style;
  if (_opts & ConsoleOutput_ColorBit) {
    switch (msg.severity) {
      case ConsoleOutput_SeverityTrace:
        style = fmt::fg(fmt::color::dim_gray);
        break;
      case ConsoleOutput_SeverityVerbose:
        style = fmt::fg(fmt::color::dim_gray);
        break;
      case ConsoleOutput_SeverityInfo:
        style = fmt::fg(fmt::color::sky_blue);
        break;
      case ConsoleOutput_SeverityWarning:
        style = fmt::emphasis::italic | fmt::fg(fmt::color::yellow);
        break;
      case ConsoleOutput_SeverityError:
        style = fmt::emphasis::italic | fmt::emphasis::bold |
                fmt::fg(fmt::color::orange_red);
        break;
      case ConsoleOutput_SeverityFatal:
        style = fmt::emphasis::italic | fmt::fg(fmt::color::white) |
                fmt::bg(fmt::color::dark_red);
        break;
      default:
        break;
    }
  }

  if (msg.ctx != nullptr) {
    return fmt::format(style, "{} [{}]", console_severity_to_str(msg.severity),
                       msg.ctx);
  } else {
    return fmt::format(style, "{}", console_severity_to_str(msg.severity));
  }
}

std::string ConsoleOutput::_format_body(const ConsoleMessage& msg) {
  std::string body;
  body.append(fmt::format(
      "{}", (_opts & ConsoleOutput_BreakAfterHeaderBit) ? "\n" : " "));
  bool include_meta_info = false;
  if (msg.severity > ConsoleOutput_SeverityInfo) {
    include_meta_info = true;
    if (_opts & ~ConsoleOutput_FilterFileBit) {
      body.append(fmt::format("file={} ", msg.file));
    }
    if (_opts & ~ConsoleOutput_FilterLineBit) {
      body.append(fmt::format("line={} ", msg.line));
    }
    if (_opts & ~ConsoleOutput_FilterFunctionBit) {
      body.append(fmt::format("func={} ", msg.fn));
    }
  }
  if (include_meta_info) {
    body.append(fmt::format(
        "{}{}", (_opts & ConsoleOutput_BreakAfterInfoBit) ? "\n" : "",
        msg.msg));
  } else {
    body.append(msg.msg);
  }
  return body;
}

ConsoleTerminalOutput::ConsoleTerminalOutput(int flags)
    : ConsoleOutput(flags) {
}

void ConsoleTerminalOutput::print_output(const ConsoleMessage& msg) {
  FILE* out = stdout;
  if (msg.severity > ConsoleOutput_SeverityWarning) {
    out = stderr;
  }
  fmt::println(out, "{}{}", _format_head(msg), _format_body(msg));
  if (_opts & ConsoleOutput_FlushPerMessageBit) {
    std::fflush(out);
  }
}

void Console::print_to_outputs(int line, const std::string& msg,
                               const char* file, const char* fn,
                               const char* ctx, int severity) {
  ConsoleMessage message {
      .line     = line,
      .msg      = msg,
      .file     = file,
      .fn       = fn,
      .ctx      = ctx,
      .severity = severity,
  };
  for (ConsoleOutput* output : s_ptr->_outputs) {
    if (output->opts() & severity) {
      output->print_output(message);
    }
  }
}

Console::Console() {
  s_ptr = this;
}

void Console::destroy() {
  for (ConsoleOutput* output : _outputs) {
    delete output;
  }
}

void Console::add_output(ConsoleOutput* output) {
  _outputs.push_back(output);
}
