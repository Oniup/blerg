file(GLOB_RECURSE BLERG_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE BLERG_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

add_executable(blerg
    ${BLERG_SOURCES}
    ${BLERG_HEADERS}
)

target_include_directories(blerg
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${blerg_core_INCLUDE_DIRS}
)
target_link_libraries(blerg
    PUBLIC
        blerg_core
)
