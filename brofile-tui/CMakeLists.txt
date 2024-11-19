cmake_minimum_required(VERSION 3.12)

include(FetchContent)

# --- Fetch FTXUI ---
set(FTXUI_ENABLE_INSTALL OFF)
set(FTXUI_BUILD_EXAMPLES OFF)
set(FTXUI_BUILD_DOCS OFF)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG 70bc44d28b36bc7b7332a88e19e9ad99c270074b
)

FetchContent_MakeAvailable(ftxui)

# --- Fetch spdlog ---
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)

FetchContent_MakeAvailable(spdlog)

# --- brofile-tui ---
project(brofile-tui LANGUAGES CXX VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 17)

add_executable(brofile-tui
        src/main.cpp
        include/brofile-tui/brofile_tui.hpp
        src/brofile_tui.cpp)

target_include_directories(brofile-tui PRIVATE include)
target_link_libraries(brofile-tui PRIVATE spdlog::spdlog ftxui::screen ftxui::dom ftxui::component brofile app-context)

# --- Install ---
set(CMAKE_INSTALL_PREFIX /usr)
install(TARGETS brofile-tui DESTINATION bin)
install(FILES brofile-tui.desktop DESTINATION share/applications)
