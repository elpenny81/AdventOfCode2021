# CMakeList.txt: CMake-Projektdatei der obersten Ebene. Führen Sie hier die globale Konfiguration aus,
# und schließen Sie Unterprojekte ein.
#
cmake_minimum_required (VERSION 3.8)

project ("AdventOfCode2021")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AoC_01_1	aoc01_1.cpp)
add_executable(AoC_01_2 aoc01_2.cpp)
add_executable(AoC_02_1	aoc02_1.cpp)
add_executable(AoC_02_2 aoc02_2.cpp)
add_executable(AoC_03_1 aoc03_1.cpp)
add_executable(AoC_03_2 aoc03_2.cpp)

configure_file(input_files/aoc01.txt ${CMAKE_BINARY_DIR}/aoc01.txt COPYONLY)
configure_file(input_files/aoc01_sample.txt ${CMAKE_BINARY_DIR}/aoc01_sample.txt COPYONLY)
configure_file(input_files/aoc02.txt ${CMAKE_BINARY_DIR}/aoc02.txt COPYONLY)
configure_file(input_files/aoc02_sample.txt ${CMAKE_BINARY_DIR}/aoc02_sample.txt COPYONLY)
configure_file(input_files/aoc03.txt ${CMAKE_BINARY_DIR}/aoc03.txt COPYONLY)
configure_file(input_files/aoc03_sample.txt ${CMAKE_BINARY_DIR}/aoc03_sample.txt COPYONLY)
