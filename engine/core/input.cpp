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

#include "core/input.h"
#include "core/console.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static Input* s_instance = nullptr;

#define KY_REG_ONCE_IMPL(func, input_type, code, pressed)                      \
  bool result = func(code);                                                    \
  if (result) {                                                                \
    result = s_instance->_register_once(input_type, code, pressed);            \
  }                                                                            \
  return result

#define KY_REG_ONCE_WITH_HANDLE_IMPL(func, handle, input_type, code, pressed)  \
  bool result = func(handle, code);                                            \
  if (result) {                                                                \
    result = s_instance->_register_once(input_type, code, pressed);            \
  }                                                                            \
  return result

Input::Input(WindowHandle* window) {
  s_instance              = this;
  s_instance->_window_ptr = window;
  for (RegisteredInput& reg : s_instance->_reg_buffer) {
    reg.type = InputType::Unknown;
  }
}

bool Input::key_pressed(KeyCode code) {
  KY_REG_ONCE_IMPL(key_press, InputType::Keyboard, code, true);
}

bool Input::key_released(KeyCode code) {
  KY_REG_ONCE_IMPL(key_release, InputType::Keyboard, code, false);
}

bool Input::key_press(KeyCode code) {
  return glfwGetKey(s_instance->_window_ptr->handle_ptr(), code) == GLFW_PRESS;
}

bool Input::key_release(KeyCode code) {
  return glfwGetKey(s_instance->_window_ptr->handle_ptr(), code) ==
         GLFW_RELEASE;
}

bool Input::mouse_pressed(MouseButton button) {
  KY_REG_ONCE_IMPL(mouse_press, InputType::Mouse, button, true);
}

bool Input::mouse_released(MouseButton button) {
  KY_REG_ONCE_IMPL(mouse_release, InputType::Mouse, button, false);
}

bool Input::mouse_press(MouseButton button) {
  return glfwGetMouseButton(s_instance->_window_ptr->handle_ptr(), button) ==
         GLFW_PRESS;
}

bool Input::mouse_release(MouseButton button) {
  return glfwGetMouseButton(s_instance->_window_ptr->handle_ptr(), button) ==
         GLFW_RELEASE;
}

std::string_view Input::type_to_string(InputType type) {
  switch (type) {
  case InputType::Keyboard:
    return std::string_view("Keyboard");
  case InputType::Mouse:
    return std::string_view("Mouse");
  case InputType::GamePad:
    return std::string_view("Game Pad");
  default:
    return std::string_view("Unknown");
  }
}

std::string_view Input::mouse_mode_to_string(MouseMode mode) {
  switch (mode) {
  case MouseMode::Visable:
    return std::string_view("Visable");
  case MouseMode::Hidden:
    return std::string_view("Hidden");
  case MouseMode::Captured:
    return std::string_view("Captured");
  default:
    return std::string_view("Invalid");
  }
}

void Input::poll_events() {
  glfwPollEvents();
  size_t reg_counted = 0;
  for (RegisteredInput& reg : s_instance->_reg_buffer) {
    if (reg_counted == s_instance->_reg_active_count) {
      break;
    } else if (reg.type == InputType::Unknown) {
      continue;
    }
    reg_counted++;

    if (!reg.rm_nx_frame) {
      reg.rm_nx_frame = true;
    } else {
      reg.type = InputType::Unknown;
      s_instance->_reg_active_count--;
    }
  }
}

bool Input::_register_once(InputType type, int code, bool pressed) {
  int free_index = -1;
  for (size_t i = 0; i < _reg_buffer.size(); i++) {
    if (_reg_buffer[i].type == InputType::Unknown) {
      free_index = i;
      continue;
    }
    bool found = _reg_buffer[i].type == type && _reg_buffer[i].code == code &&
                 _reg_buffer[i].pressed == pressed;
    if (found) {
      _reg_buffer[i].rm_nx_frame = false;
      return false;
    }
  }

  CONTEXT_CONDITION_ERROR_RETURN(
      "INPUT", free_index != -1, false,
      "Toggle register buffer full, cannot add {} {}", type_to_string(type),
      code);

  _reg_buffer[free_index] = RegisteredInput {
      .type        = type,
      .code        = code,
      .rm_nx_frame = false,
      .pressed     = pressed,
  };
  _reg_active_count++;
  return true;
}
