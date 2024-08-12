option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)

add_subdirectory(glfw)

target_sources(core_runtime
  # ImGui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imstb_truetype.h

  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/backends/imgui_impl_glfw.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/imgui_widgets.cpp

  # GLAD
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)
target_include_directories(core_runtime
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/
    ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include/

    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include/
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/
)
target_compile_definitions(core_runtime
  PUBLIC
    FMT_HEADER_ONLY
)
target_link_libraries(core_runtime
  PRIVATE
    glfw
)
