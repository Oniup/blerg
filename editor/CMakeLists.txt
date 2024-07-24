file(GLOB_RECURSE IGNITE_EDITOR_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE IGNITE_EDITOR_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

add_executable(ignite_editor
    ${IGNITE_EDITOR_SOURCES}
    ${IGNITE_EDITOR_HEADERS}
)

target_include_directories(ignite_editor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ignite_INCLUDE_DIRS}
)
target_link_libraries(ignite_editor
    PUBLIC
        ignite
)
