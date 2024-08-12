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

#ifndef GFX_RHI_BASE_WINDOW_HANDLE_H
#define GFX_RHI_BASE_WINDOW_HANDLE_H

#include <glm/glm.hpp>
#include <string_view>

struct GLFWwindow;

enum WindowHandleFlags {
  WindowHandle_NoneBit              = 0,
  WindowHandle_WindowedModeBit      = 1 << 0,
  WindowHandle_BorderlessModeBit    = 1 << 1,
  WindowHandle_FullscreenModeBit    = 1 << 2,
  WindowHandle_VsyncBit             = 1 << 3,
  WindowHandle_TrippleBufferBit     = 1 << 4,
  WindowHandle_ResizableBit         = 1 << 5,
  WindowHandle_TransparentBufferBit = 1 << 6,
};

class WindowHandle {
public:
  static constexpr int s_DefaultOptions = WindowHandle_WindowedModeBit |
                                          WindowHandle_ResizableBit |
                                          WindowHandle_VsyncBit;

public:
  WindowHandle(const std::string_view& title, int32_t width = -1,
               int32_t height = -1, int flags = WindowHandle::s_DefaultOptions);

  inline GLFWwindow* handle_ptr() { return _win_ptr; }
  inline const GLFWwindow* handle_ptr() const { return _win_ptr; }

  glm::ivec2 size() const;
  glm::ivec2 position() const;
  glm::ivec2 framebuffer_size() const;
  std::string_view title() const;

  void destroy();
  void swap_buffers();
  bool closing() const;
  void make_current_context();

  inline int& options() { return _opts; }
  inline int options() const { return _opts; }

private:
  virtual bool _initialize(const std::string_view& title, int32_t width,
                           int32_t height, int flags = s_DefaultOptions);
  static bool _valid_options(int opts);

  GLFWwindow* _win_ptr = nullptr;
  int _opts            = WindowHandle::s_DefaultOptions;
};

#endif
