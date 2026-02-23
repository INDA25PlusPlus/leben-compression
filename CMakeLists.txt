cmake_minimum_required(VERSION 3.24)
project(leben_compression C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(leben_compression src/main.c
        src/bitmap.c
        src/bitmap.h)
