cmake_minimum_required(VERSION 3.27)

include(FetchContent)

# --- Fetch FTXUI ---
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v4.0.0
)

FetchContent_MakeAvailable(ftxui)

# --- brofile-tui ---
project(brofile-tui LANGUAGES CXX VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 17)

add_executable(brofile-tui
        src/main.cpp
        include/brofile-tui/brofile_tui.hpp
        src/brofile_tui.cpp)

target_include_directories(brofile-tui PRIVATE include)
target_link_libraries(brofile-tui PRIVATE ftxui::screen ftxui::dom ftxui::component brofile)

# --- Install ---
set(CMAKE_INSTALL_PREFIX /usr)
install(TARGETS brofile-tui DESTINATION bin)
install(FILES brofile-tui.desktop DESTINATION share/applications)
