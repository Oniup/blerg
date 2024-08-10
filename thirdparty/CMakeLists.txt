# Options
# -------------------------------------------------------------------------------
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)

# Sources
# -------------------------------------------------------------------------------
add_subdirectory(glfw)

# Glad
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glad)
target_sources(fiwre_runtime
  PRIVATE
    ${GLAD_DIR}/include/glad/glad.h
    ${GLAD_DIR}/include/KHR/khrplatform.h
    ${GLAD_DIR}/src/glad.c
)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui)
target_sources(fiwre_runtime
  PUBLIC
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h

  PRIVATE
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Linking
# -------------------------------------------------------------------------------
target_include_directories(fiwre_runtime
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include

  PRIVATE
    ${glfw_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)
target_compile_definitions(fiwre_runtime
  PUBLIC
    FMT_HEADER_ONLY
)
target_link_libraries(fiwre_runtime
  PRIVATE
    glfw
)
