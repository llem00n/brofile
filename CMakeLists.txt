cmake_minimum_required(VERSION 3.27)
project(brofile)

add_subdirectory(libbrofile)

add_executable(main main.cpp)
target_link_libraries(main libbrofile)