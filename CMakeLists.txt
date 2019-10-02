cmake_minimum_required(VERSION 3.0.0)
project(spacer VERSION 0.1.0)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})


set(CMAKE_CXX_STANDARD 14)
add_executable(spacer src/main.cc src/spacer.cc)
target_include_directories(spacer PUBLIC ${ROOT_DIR}/include)


