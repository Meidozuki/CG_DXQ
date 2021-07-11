cmake_minimum_required(VERSION 3.17)
project(GLUTDemo)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/eigen3)
link_directories(lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(GLUTDemo main.cpp)

target_link_libraries(GLUTDemo freeglut openGL32 glu32)