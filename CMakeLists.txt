cmake_minimum_required(VERSION 3.28)
project(EtoVision)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)


include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(EtoVision main.cpp)

target_link_libraries( EtoVision ${OpenCV_LIBS} )