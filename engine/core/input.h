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

#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include "core/input_key_codes.h"
#include "gfx_rhi/window_handle.h"
#include <array>
#include <fmt/format.h>
#include <string_view>

enum InputType {
  Unknown = -1,
  Keyboard,
  Mouse,
  GamePad,
};

enum MouseMode {
  Invalid = -1,
  Visable,
  Hidden,
  Captured,
};

struct RegisteredInput {
  InputType type   = InputType::Unknown;
  int code         = -1;
  bool rm_nx_frame = false; /// Remove next frame?
  bool pressed     = false;
};

struct Input {
  static constexpr size_t RegisterOnceBufferSize = 16;

  Input(WindowHandle* manager_ptr);

  static bool key_pressed(KeyCode code);
  static bool key_released(KeyCode code);

  static bool key_press(KeyCode code);
  static bool key_release(KeyCode code);

  static bool mouse_pressed(MouseButton button);
  static bool mouse_released(MouseButton button);

  static bool mouse_press(MouseButton button);
  static bool mouse_release(MouseButton button);

  static std::string_view type_to_string(InputType type);
  static std::string_view mouse_mode_to_string(MouseMode mode);

  void poll_events();

private:
  bool _register_once(InputType type, int code, bool pressed);

private:
  WindowHandle* _window_ptr = nullptr;
  size_t _reg_active_count  = 0;
  std::array<RegisteredInput, RegisterOnceBufferSize> _reg_buffer;
};

#endif
