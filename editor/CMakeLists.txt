file(GLOB_RECURSE SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

add_executable(fiwre
  ${SOURCES}
  ${HEADERS}
)

target_include_directories(fiwre
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${core_runtime_INCLUDE_DIRS}
)
target_link_libraries(fiwre
  PUBLIC
    core_runtime
)
