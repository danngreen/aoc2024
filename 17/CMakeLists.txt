cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_BUILD_TYPE "RelWithDebInfo")
set(CMAKE_BUILD_TYPE "Debug")

project(
  AOC2024
  VERSION 0.10
  DESCRIPTION "Advent of Code 2024"
  LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 26)

add_executable(AOC
    main.cc
)
