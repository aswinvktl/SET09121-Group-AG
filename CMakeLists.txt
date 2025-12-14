cmake_minimum_required(VERSION 3.11)
project(GhostRedemption)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# --- SFML ---
add_subdirectory(lib/SFML)
set(SFML_INCS lib/SFML/include)

# --- Auto-discover all cpp files ---
file(GLOB_RECURSE GAME_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(game
    ${GAME_SOURCES}
)

target_include_directories(game PRIVATE
    src
    ${SFML_INCS}
)

target_link_libraries(game
    sfml-graphics
    sfml-window
    sfml-system
)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:game>/resources
)
