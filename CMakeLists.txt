cmake_minimum_required(VERSION 3.20)

# CMake Policies
# ------------------------------------------------------------------------------
cmake_policy(SET CMP0003 NEW) # Libraries linked via full path != linker search
# path error.
cmake_policy(SET CMP0010 NEW) # Syntax problems are errors.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

# CMake Project
# ------------------------------------------------------------------------------
project(fiwre)
set(_VERSION_BUILD_INFO
  "0.0.1"
  CACHE INTERNAL "")

# FIWRE build options
# ------------------------------------------------------------------------------
option(BUILD_FIWRE_LIB "Core runtime static library" ON)
option(BUILD_FIWRE_EDITOR_EXE "Build Editor Executable" ON)

# Build directories
# ------------------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)

# Project source files
# ------------------------------------------------------------------------------
if(${BUILD_FIWRE_LIB})
  add_library(core_runtime STATIC)
  add_subdirectory(thirdparty)
  add_subdirectory(engine)
endif()

if(${BUILD_FIWRE_EDITOR_EXE})
  add_subdirectory(editor)
endif()
