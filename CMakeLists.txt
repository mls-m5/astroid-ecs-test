
cmake_minimum_required(VERSION 3.16)
project(game-2022-06)

add_subdirectory(lib/entt)
add_subdirectory(lib/sdlpp)

add_executable(
    game-2022-06
    src/main.cpp
    src/game.cpp
    src/components.cpp
    src/drawcomponents.cpp
    src/controls.cpp
    )

target_compile_features(
    game-2022-06
    PRIVATE
    cxx_std_17
    )

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    sdlpp
    EnTT::EnTT
    )

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(
        ${PROJECT_NAME}
        PUBLIC
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html
        )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2")

endif()
