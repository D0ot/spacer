cmake_minimum_required(VERSION 3.0.0)
project(spacer VERSION 0.1.0)

find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 14)
add_executable(spacer src/main.cc src/spacer.cc)
target_include_directories(spacer PUBLIC include)

add_executable(spacergui src/gui.cc src/spacer.cc src/glad.c src/shader.cc)
target_link_libraries(spacergui PUBLIC glfw ${CMAKE_DL_LIBS})
target_include_directories(spacergui PUBLIC include)
target_compile_definitions(spacergui PUBLIC ROOT_DIR)


