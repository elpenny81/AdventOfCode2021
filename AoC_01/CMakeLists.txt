cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
	AoC_01_1
	aoc01_1.cpp
)

add_executable(
    AoC_01_2
    aoc01_2.cpp
)

add_custom_command(
        TARGET AoC_01_1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/AoC_01/input.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
        TARGET AoC_01_2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/AoC_01/input.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)
