file(GLOB_RECURSE fiwre_runtime_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE fiwre_runtime_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

target_sources(fiwre_runtime
  PUBLIC  ${fiwre_runtime_SOURCES}
  PRIVATE ${fiwre_runtime_HEADERS}
)
target_include_directories(fiwre_runtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
