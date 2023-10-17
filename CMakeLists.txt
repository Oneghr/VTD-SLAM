cmake_minimum_required(VERSION 2.8)
project(ORB_SLAM2)
include_directories(/usr/include/)
find_package(OpenCV QUIET)
add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})


