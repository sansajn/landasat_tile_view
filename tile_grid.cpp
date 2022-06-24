#include "tile_grid.hpp"

constexpr size_t tile_size = 256;

tile_grid::tile_grid(size_t rows, size_t cols, tile_item * tiles)
	: _tiles(rows) {
	for (grid_row & row : _tiles)
		row.resize(cols);
}
