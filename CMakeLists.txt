cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(PixelSandbox VERSION 1.0)

add_subdirectory(World)

find_package(SDL2 REQUIRED)

add_executable(PixelSandbox main.cpp)
target_link_libraries(PixelSandbox PUBLIC World ${SDL2_LIBRARIES})
