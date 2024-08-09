find_package(GTest)
if(NOT ${GTest_FOUND})
  message(WARN
    "Google testing framework not installed which is required for compiling and "
    "running the Kryos tests")
else()
  add_executable(tests)
  target_include_directories(tests
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${fiwre_runtime_INCLUDE_DIRS}
      ${gtest_INCLUDE_DIRS}
  )
  target_link_libraries(tests
    PRIVATE
      fiwre_runtime
      GTest::GTest
  )

  file(GLOB_RECURSE TEST_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
  target_sources(tests
    PRIVATE
      ${TEST_SOURCES}
  )

  # Automatically discover the tests
  include(GoogleTest)
  gtest_discover_tests(tests)
endif()

