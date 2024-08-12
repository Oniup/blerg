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

#include "core/console.h"
#include "core/input.h"
#include "gfx_rhi/window_handle.h"
#include <cstdio>

int main() {
  Console console;
  console.add_output<ConsoleTerminalOutput>(
      ConsoleOutput::s_DefaultOptions | ConsoleOutput::s_DefaultSeverity |
      ConsoleOutput_SeverityInfoBit | ConsoleOutput_SeverityVerboseBit |
      ConsoleOutput_SeverityTraceBit);

  WindowHandle window("Engine Editor");
  Input input(&window);

  while (!window.closing()) {
    window.swap_buffers();
    input.poll_events();
  }

  window.destroy();
  console.destroy();
  return 0;
}
