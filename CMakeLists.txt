# developed on Ubuntu 22.04 LTS
# dependencies: libspdlog-dev libboost-dev-all libtiff5-dev
cmake_minimum_required(VERSION 3.16)

project(ltview VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(TIFF 4.3.0 REQUIRED)
find_package(Boost 1.74 REQUIRED)
find_package(spdlog 1.9.2 REQUIRED)
find_package(Catch2 2.13 REQUIRED)

add_executable(ltview
	main.cpp
	tile_item.hpp
	tile_item.cpp
	tile_grid.cpp
	tile_grid.hpp
	tile_stats.cpp
	tile_stats.hpp
	tiff_tiled_file.hpp
	tiff_tiled_file.cpp
	checker.cpp
	checker.hpp
)

add_executable(test
	test_grid.cpp
)

target_link_libraries(ltview PRIVATE Qt6::Widgets TIFF::TIFF spdlog::spdlog)
