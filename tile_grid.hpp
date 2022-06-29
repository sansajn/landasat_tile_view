#pragma once
#include <algorithm>
#include <deque>
#include <vector>
#include <cassert>

/*! Grid of tiles on scren.
Tile grid structure to support moving tiles on screen while map scrolling/panning. */
template <typename Tile>
class tile_grid {
public:
	tile_grid(size_t rows, size_t cols, Tile * tiles);  // TODO: do not use raw pointers there, using tiles is error prome
	std::vector<Tile *> front_row() const;
	std::vector<Tile *> back_row() const;
	std::vector<Tile *> front_column() const;
	std::vector<Tile *> back_column() const;
	void move_front_row_back();
	void move_back_row_front();
	void move_front_column_back();

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

template <typename Tile>
std::vector<Tile *> tile_grid<Tile>::back_row() const {
	auto & back = _tiles.back();
	std::vector<Tile *> result(std::size(back));
	std::copy(begin(back), end(back), begin(result));
	return result;
}

template <typename Tile>
std::vector<Tile *> tile_grid<Tile>::front_column() const {
	std::vector<Tile *> result;
	result.reserve(std::size(_tiles));

	for (grid_row const & row : _tiles)  // TODO: can we use copy transform range there?
		result.push_back(row.front());

	return result;
}

template <typename Tile>
std::vector<Tile *> tile_grid<Tile>::back_column() const {
	std::vector<Tile *> result;
	result.reserve(std::size(_tiles));

	for (grid_row const & row : _tiles)  // TODO: can we use copy transform range there?
		result.push_back(row.back());

	return result;
}

template <typename Tile>
void tile_grid<Tile>::move_front_row_back() {
	grid_row front = _tiles.front();
	_tiles.pop_front();
	_tiles.push_back(std::move(front));
}

template <typename Tile>
void tile_grid<Tile>::move_back_row_front() {
	grid_row back = _tiles.back();
	_tiles.pop_back();
	_tiles.push_front(std::move(back));
}

template <typename Tile>
void tile_grid<Tile>::move_front_column_back() {
	for (grid_row & row : _tiles) {  // TODO: can we use transform there?
		Tile * tile = row.front();
		row.pop_front();
		row.push_back(tile);
	}
}
