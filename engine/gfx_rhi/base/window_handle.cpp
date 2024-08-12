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

#define GLFW_INCLUDE_NONE

#include "gfx_rhi/window_handle.h"
#include "core/console.h"
#include <GLFW/glfw3.h>

static bool s_GlfwInitialized = false;

static void _glfw_error_callback(int error, const char* description) {
  CONTEXT_ERROR("GLFW", "Error Number {}: {}", error, description);
}

static void _initialize_glfw() {
  CONTEXT_CONDITION_FATAL("GLFW", !s_GlfwInitialized,
                          "Cannot initialize more than one GLFW instance");
  glfwInit();
  glfwSetErrorCallback(_glfw_error_callback);
  s_GlfwInitialized = true;
}

static void _destroy_glfw() {
  CONTEXT_CONDITION_ERROR("GLFW", s_GlfwInitialized,
                          "Cannot destroy GLFD there is no instance active");
  glfwTerminate();
  s_GlfwInitialized = false;
}

WindowHandle::WindowHandle(const std::string_view& title, int32_t width,
                           int32_t height, int flags) {
  _initialize_glfw();
  _initialize(title, width, height, flags);
}

glm::ivec2 WindowHandle::size() const {
  glm::ivec2 win_size;
  glfwGetWindowSize(_win_ptr, &win_size.x, &win_size.y);
  return win_size;
}

glm::ivec2 WindowHandle::position() const {
  glm::ivec2 win_pos;
  glfwGetWindowPos(_win_ptr, &win_pos.x, &win_pos.y);
  return win_pos;
}

glm::ivec2 WindowHandle::framebuffer_size() const {
  glm::ivec2 buffer_size;
  glfwGetFramebufferSize(_win_ptr, &buffer_size.x, &buffer_size.y);
  return buffer_size;
}

std::string_view WindowHandle::title() const {
  return std::string_view(glfwGetWindowTitle(_win_ptr));
}

void WindowHandle::destroy() {
  glfwDestroyWindow(_win_ptr);
  _destroy_glfw();

  _win_ptr = nullptr;
  _opts    = WindowHandle_NoneBit;
}

bool WindowHandle::closing() const {
  return glfwWindowShouldClose(_win_ptr);
}

void WindowHandle::make_current_context() {
  glfwMakeContextCurrent(_win_ptr);
}

bool WindowHandle::_valid_options(int opts) {
  int target = WindowHandle_WindowedModeBit | WindowHandle_BorderlessModeBit |
               WindowHandle_FullscreenModeBit;
  int enabled = target & opts;
  return enabled == 0 || enabled == WindowHandle_WindowedModeBit ||
         enabled == WindowHandle_BorderlessModeBit ||
         enabled == WindowHandle_FullscreenModeBit;
}
