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

#ifdef RHI_USE_OPENGL

#define GLFW_INCLUDE_NONE
#include "gfx_rhi/window_handle.h"
#include "core/console.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

static bool s_GladInitialized = false;

bool WindowHandle::_initialize(const std::string_view& title, int32_t width,
                               int32_t height, int flags) {
  RHI_CONDITION_FATAL(title[title.size()] == '\0',
                      "Title string must be null terminated");
  RHI_CONDITION_FATAL(
      _valid_options(flags),
      "Can only create window with one or none of the window modes");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RESIZABLE, (flags & WindowHandle_ResizableBit) != 0);
  glfwWindowHint(GLFW_DECORATED,
                 (flags & ~WindowHandle_BorderlessModeBit) != 0);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                 (flags & WindowHandle_TransparentBufferBit) != 0);

  GLFWmonitor* monitor       = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  if (width < 0 | height < 0) {
    width  = vidmode->width / 2;
    height = vidmode->height / 2;
  }

  if (flags & ~WindowHandle_FullscreenModeBit) {
    monitor = nullptr;
  }

  GLFWwindow* window =
      glfwCreateWindow(width, height, title.data(), monitor, nullptr);
  RHI_CONDITION_FATAL(window != nullptr, "Failed to create GLFW window");
  glfwMakeContextCurrent(window);

  if (!s_GladInitialized) {
    s_GladInitialized = true;
    RHI_CONDITION_FATAL(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
                        "Failed to initialize GLAD");
  }

  if (flags & WindowHandle_VsyncBit) {
    glfwSwapInterval(0);
  } else {
    glfwSwapInterval(1);
  }

  _win_ptr = window;
  _opts    = flags;
  return false;
}

void WindowHandle::swap_buffers() {
  glfwSwapBuffers(_win_ptr);
}

#endif
