cmake_minimum_required(VERSION 3.11)
project(GhostRedemption)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_subdirectory(lib/SFML)
set(SFML_INCS lib/SFML/include)

add_executable(game
    src/main.cpp
    src/levels.cpp
    src/systems/GameSystem.cpp
    src/scenes/CemeteryScene.cpp
    src/scenes/Level1Scene.cpp
    src/scenes/Level2Scene.cpp
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
