cmake_minimum_required(VERSION 3.27)
project(app-context CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(app-context STATIC include/app_context/app_context.hpp src/app_context.cpp)
target_include_directories(app-context PUBLIC include)
target_include_directories(app-context INTERFACE include)
