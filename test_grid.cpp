// unit tests for tile_grid implementation
#include <vector>
#include <catch.hpp>
#include "tile_grid.hpp"

using std::vector;

TEST_CASE("tile_grid: we can create grid from list of tiles",
	"[tile_grid") {
	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{&tiles[0], &tiles[1], &tiles[2], &tiles[3]};

	// ACT
	tile_grid grid{2, 2, tiles};

	// CHECK
	REQUIRE(true);
}

TEST_CASE("tile_grid: we can get row of tiles",
	"[tile_grid]") {
	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{
		&tiles[0], &tiles[1],
		&tiles[2], &tiles[3]};

	tile_grid grid{2, 2, tiles};

	// ACT & CHECK
	SECTION("from front") {
		vector<int *> const expected{&tiles[0], &tiles[1]};
		vector<int *> row = grid.front_row();
		REQUIRE(row == expected);
	}

	SECTION("from back") {
		vector<int *> const expected{&tiles[2], &tiles[3]};
		vector<int *> row = grid.back_row();
		REQUIRE(row == expected);
	}
}

TEST_CASE("tile_grid: we can get column of tiles",
	"[tile_grid]") {
	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{
		&tiles[0], &tiles[1],
		&tiles[2], &tiles[3]};

	tile_grid grid{2, 2, tiles};

	// ACT & CHECK
	SECTION("from front") {
		vector<int *> const expected{&tiles[0], &tiles[2]};
		vector<int *> col = grid.front_column();
		REQUIRE(col == expected);
	}

	SECTION("from back") {
		vector<int *> const expected{&tiles[1], &tiles[3]};
		vector<int *> col = grid.back_column();
		REQUIRE(col == expected);
	}
}

TEST_CASE("tile_grid: we can move row of tiles",
	"[tile_grid]") {
	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{
		&tiles[0], &tiles[1],
		&tiles[2], &tiles[3]};

	tile_grid grid{2, 2, tiles};

	vector<int *> const expected_front_row{&tiles[2], &tiles[3]},
		expected_back_row{&tiles[0], &tiles[1]};

	// ACT & CHECK
	SECTION("from front to back") {
		grid.move_front_row_back();
		REQUIRE(grid.front_row() == expected_front_row);
		REQUIRE(grid.back_row() == expected_back_row);
	}

	SECTION("from back to front") {
		grid.move_back_row_front();
		REQUIRE(grid.front_row() == expected_front_row);
		REQUIRE(grid.back_row() == expected_back_row);
	}
}

TEST_CASE("tile_grid: we can move column of tiles",
	"[tile_grid]") {
	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{
		&tiles[0], &tiles[1],
		&tiles[2], &tiles[3]};

	tile_grid grid{2, 2, tiles};

	vector<int *> const expected_front_col{&tiles[1], &tiles[3]},
		expected_back_col{&tiles[0], &tiles[2]};

	// ACT & CHECK
	SECTION("from front to back") {
		grid.move_front_column_back();
		REQUIRE(grid.front_column() == expected_front_col);
		REQUIRE(grid.back_column() == expected_back_col);
	}

	SECTION("from back to front") {
		grid.move_back_column_front();
		REQUIRE(grid.front_column() == expected_front_col);
		REQUIRE(grid.back_column() == expected_back_col);
	}
}
