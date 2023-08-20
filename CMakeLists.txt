cmake_minimum_required(VERSION 3.23)
project(SpeedyGo)

set(CMAKE_CXX_STANDARD 17)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

add_subdirectory(src)