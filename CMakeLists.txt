cmake_minimum_required(VERSION 3.14)
project(app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

add_executable(app src/main.c src/chip8.c src/chip8.h)
target_link_libraries(app PRIVATE raylib)
