cmake_minimum_required(VERSION 3.27)
project(brofile)

set(CLIENT_TYPE "ALL" CACHE STRING "Desired TUI/GUI client type (now it's TUI only)")
if(tui)
    set(CLIENT_TYPE "TUI")
    message(STATUS "Configuring for TUI")
endif()

if(CLIENT_TYPE STREQUAL "ALL")
    message(STATUS "Configuring for ALL")
endif()

add_subdirectory(libbrofile)
add_subdirectory(app-context)

if(CLIENT_TYPE STREQUAL "ALL" OR CLIENT_TYPE STREQUAL "TUI")
    add_subdirectory(brofile-tui)
endif()