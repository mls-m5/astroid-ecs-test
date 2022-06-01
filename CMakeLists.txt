
cmake_minimum_required(VERSION 3.23)
project(game-2022-06)

add_subdirectory(lib/entt)
add_subdirectory(lib/sdlpp)

add_executable(
    game-2022-06
    src/main.cpp
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
    )
