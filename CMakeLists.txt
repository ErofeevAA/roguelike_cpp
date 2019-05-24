cmake_minimum_required(VERSION 3.13)
project(roguelike)

set(CMAKE_CXX_STANDARD 17)

add_executable(roguelike main.cpp objects.cpp objects.h game.cpp game.h config_reader.h)

find_package( Curses REQUIRED )

include_directories( ${CURSES_INCLUDE_DIRS} )
target_link_libraries( roguelike ${CURSES_LIBRARIES} )