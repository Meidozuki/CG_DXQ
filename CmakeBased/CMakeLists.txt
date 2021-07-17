cmake_minimum_required(VERSION 3.17)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

#opencv package
find_package(OpenCV REQUIRED
        PATHS 3rd/opencv)
include_directories(3rd/opencv/include)

#eigen package
#find_package(Eigen3 REQUIRED
#        PATHS 3rd/eigen/share)
include_directories(3rd/eigen/include/eigen3)

list(APPEND SRC_CPP
        main.cpp  rasterizer.cpp Triangle.cpp
        viewmodel.cpp Message.cpp view.cpp
        )

#conveniece to link shared libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(Rasterizer
        ${SRC_CPP}
        )

target_link_libraries(Rasterizer
        ${OpenCV_LIBS}
        )

#specify CUR_SRC_DIR
configure_file(config.h.in config.h)
target_include_directories(Rasterizer PUBLIC "${PROJECT_BINARY_DIR}")