cmake_minimum_required(VERSION 3.15)
project(MiniSearch CXX)

# Force modern C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Automatically collect ALL .cpp source files inside the src/ folder
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 2. Define the executable using the dynamic file list
add_executable(mini_search ${SOURCES})

# 3. Add strict compiler flags to catch bugs early (perfect for debugging)
if(MSVC)
    target_compile_options(mini_search PRIVATE /W4)
else()
    target_compile_options(mini_search PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Ensure the executable can locate the data/ folder correctly
target_include_directories(mini_search PRIVATE src)