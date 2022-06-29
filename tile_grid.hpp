#pragma once
#include <algorithm>
#include <deque>
#include <vector>
#include <cassert>

/*! Grid of tiles on scren.
The structure can handle operations with grid of tiles visible on screen. */
template <typename Tile>
class tile_grid {
public:
	tile_grid(size_t rows, size_t cols, Tile * tiles);  // TODO: do not use raw pointers there, using tiles is error prome
	std::vector<Tile *> front_row() const;


private:
	using grid_row = std::deque<Tile *>;
	std::deque<grid_row> _tiles;
};

template <typename Tile>
tile_grid<Tile>::tile_grid(size_t rows, size_t cols, Tile * tiles) {
	// populate grid
	Tile * tile_it = tiles;
	for (size_t i = 0; i < rows; ++i) {
		grid_row row;
		for (size_t j = 0; j < cols; ++j)
			row.push_back(tile_it++);

		assert(std::size(row) == cols);
		_tiles.push_back(std::move(row));
	}

	assert(std::size(_tiles) == rows);
}

template <typename Tile>
std::vector<Tile *> tile_grid<Tile>::front_row() const {
	auto & front = _tiles.front();
	std::vector<Tile *> result(std::size(front));
	std::copy(begin(front), end(front), begin(result));
	return result;
}
