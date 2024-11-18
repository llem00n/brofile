cmake_minimum_required(VERSION 3.27)
project(app-context CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# --- Fetch nlohmann/json ---
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

add_library(app-context STATIC include/app_context/app_context.hpp src/app_context.cpp)
target_include_directories(app-context PUBLIC include)
target_include_directories(app-context INTERFACE include)
target_link_libraries(app-context PRIVATE nlohmann_json::nlohmann_json)
