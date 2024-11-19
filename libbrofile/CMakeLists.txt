cmake_minimum_required(VERSION 3.27)

include(FetchContent)

# --- Fetch nlohmann/json ---
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

# --- brofile ---
project(brofile LANGUAGES CXX VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES include/brofile/scanner.hpp src/scanner.cpp
        include/brofile/browser_type.hpp
        include/brofile/browser_factory.hpp
        include/brofile/browser_base.hpp
        include/brofile/firefox.hpp
        src/browser_factory.cpp
        private_include/brofile_private/browsers/chromium.hpp
        private_include/brofile_private/browsers/firefox.hpp
        private_include/brofile_private/tools/ini_tools.hpp
        src/browser_base.cpp
        src/chromium.cpp
        src/firefox.cpp
        src/ini_tools.cpp
        include/brofile/browser_profile_info.hpp
        include/brofile/firefox_container_info.hpp
)

add_library(brofile SHARED ${SOURCE_FILES})

target_include_directories(brofile PUBLIC include)
target_include_directories(brofile PRIVATE private_include)
target_include_directories(brofile INTERFACE include)
target_link_libraries(brofile PRIVATE nlohmann_json::nlohmann_json)

# --- install ---
set(CMAKE_INSTALL_PREFIX /usr)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS brofile DESTINATION lib)
install(TARGETS brofile DESTINATION lib64)
