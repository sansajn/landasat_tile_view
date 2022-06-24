#pragma once
#include <deque>
#include "tile_item.hpp"

//! Grid of tiles on screen.
class tile_grid {
public:
	tile_grid(size_t rows, size_t cols, tile_item * tiles);  // TODO: do not use raw pointers there, using tiles is error prome

private:
	using grid_row = std::deque<tile_item *>;
	std::deque<grid_row> _tiles;
};
