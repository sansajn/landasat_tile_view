#include <vector>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "tile_grid.hpp"

using std::vector;

TEST_CASE("we can test with catch2", "[catch]") {
	REQUIRE(true);
}

// unit tests for tile_grid implementation

TEST_CASE("we can create tile grid from list of tiles",
	"[tile_grid") {
	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{&tiles[0], &tiles[1], &tiles[2], &tiles[3]};

	// ACT
	tile_grid grid{2, 2, tiles};

	// CHECK
	REQUIRE(true);
}

// TODO: can be merged to single test case
TEST_CASE("we can get front row tiles",
	"[tile_grid]") {

	// SETUP
	int tiles[4] = {1, 2, 3, 4};
	vector<int *> tile_list{
		&tiles[0], &tiles[1],
		&tiles[2], &tiles[3]};

	vector<int *> const expected{&tiles[0], &tiles[1]};

	tile_grid grid{2, 2, tiles};

	// ACT
	vector<int *> row = grid.front_row();

	// CHECK
	REQUIRE(row == expected);
}

TEST_CASE("we can get back row tiles") {
	// TODO: implement
}

TEST_CASE("we can get front column tiles") {
	// TODO: implement
}

TEST_CASE("we can get back column tiles") {
	// TODO: implement
}



// TODO: can be merged to single test case
TEST_CASE("we can move front row tiles to back") {
	// TODO: implement
}

TEST_CASE("we can move back row tiles to front") {
	// TODO: implement
}

TEST_CASE("we can move front column tiles to back") {
	// TODO: implement
}

TEST_CASE("we can move back column tiles to front") {
	// TODO: implement
}
