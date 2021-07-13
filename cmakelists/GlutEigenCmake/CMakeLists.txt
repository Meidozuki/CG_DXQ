cmake_minimum_required(VERSION 3.17)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 14)

#include_directories(install/include)
#link_directories(install/lib)

include_directories(include)

find_package(FreeGLUT REQUIRED
        PATHS 3rd/opengl/lib/cmake)

#find_package(Eigen3 REQUIRED
#        PATHS 3rd/eigen/share)
include_directories(3rd/eigen/include/eigen3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(Rasterizer main.cpp)

target_link_libraries(Rasterizer
        FreeGLUT::freeglut
        opengl32
        glu32)
