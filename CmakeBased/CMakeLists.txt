cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -O1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

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
        main.cpp  rasterizer.cpp Triangle.cpp ThreadPool.cpp
        viewmodel.cpp Message.cpp view.cpp
        )

project(Rasterizer)
add_executable(Rasterizer
        ${SRC_CPP}
        )

#Unicode
if(WIN32)
    set(STRING_CXX_FLAGS "-D_UNICODE")
    foreach(src ${SRC_CPP})
        set_source_files_properties(${src} PROPERTIES
                COMPILE_FLAGS "${STRING_CXX_FLAGS}"
                )
    endforeach()
endif(WIN32)

target_link_libraries(Rasterizer
        ${OpenCV_LIBS}
        )

#specify CUR_SRC_DIR
configure_file(config.h.in config.h)
target_include_directories(Rasterizer PUBLIC "${PROJECT_BINARY_DIR}")

include(CPack)