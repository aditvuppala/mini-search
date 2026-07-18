cmake_minimum_required(VERSION 3.15)
project(MiniSearch LANGUAGES CXX)

# Enforce the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Apply strict compiler flags for maximum speed and safety
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pedantic -Wconversion")
endif()

# Tell CMake to compile our initial main file
add_executable(mini_search 
    src/main.cpp 
)