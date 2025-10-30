cmake_minimum_required(VERSION 3.10)
project(projc C)

set(CMAKE_C_STANDARD 99)

add_executable(projc
        main.c
        kmeans.c
)
