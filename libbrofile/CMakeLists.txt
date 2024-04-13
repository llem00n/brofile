cmake_minimum_required(VERSION 3.27)
project(libbrofile)
set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.11.2 REQUIRED)

set(SOURCE_FILES include/brofile/scanner.hpp src/scanner.cpp
        include/brofile/browser_type.hpp
        include/brofile/browser_factory.hpp
        include/brofile/browser_base.hpp
        src/browser_factory.cpp
        private_include/brofile_private/browsers/chromium.hpp
        src/browser_base.cpp
        src/chromium.cpp
        include/brofile/browser_profile_info.hpp)

add_library(libbrofile SHARED ${SOURCE_FILES})

target_include_directories(libbrofile PUBLIC include)
target_include_directories(libbrofile PRIVATE private_include)
target_include_directories(libbrofile INTERFACE include)
target_link_libraries(libbrofile PRIVATE nlohmann_json::nlohmann_json)
