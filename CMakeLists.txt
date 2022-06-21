# developed on Ubuntu 22.04 LTS
cmake_minimum_required(VERSION 3.16)

project(ltview VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(TIFF REQUIRED)
find_package(Boost 1.74 REQUIRED)

add_executable(ltview
	main.cpp
	tile_stats.cpp
	tile_stats.hpp
	tiff_tiled_file.hpp
	tiff_tiled_file.cpp
	checker.cpp
	checker.hpp
)

target_link_libraries(ltview PRIVATE Qt6::Widgets TIFF::TIFF)
