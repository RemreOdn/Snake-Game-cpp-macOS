cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 23)

# ncurses kütüphanesini ekle
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Derlenecek dosyalar
add_executable(snake_game src/main.cpp)
target_link_libraries(snake_game ${CURSES_LIBRARIES})