cmake_minimum_required(VERSION 3.17)
project(GLUTDemo)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

link_directories(lib)
add_executable(GLUTDemo main.cpp)

target_link_libraries(GLUTDemo freeglut openGL32 glu32)