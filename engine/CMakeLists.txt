file(GLOB_RECURSE SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

target_sources(core_runtime
  PUBLIC  ${SOURCES}
  PRIVATE ${HEADERS}
)
target_include_directories(core_runtime
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(core_runtime
  PUBLIC
    RHI_USE_OPENGL
    # RHI_USE_VULKAN
)
