#include <algorithm>
#include <vector>
#include <cassert>
#include "tiff_tiled_file.hpp"
#include "checker.hpp"

using std::unique_ptr, std::vector, std::filesystem::path, std::copy;


tiff_tiled_file::tiff_tiled_file(path const & fname) {
	_tiff = TIFFOpen(fname.c_str(), "r");
	assert(_tiff);
	// TODO: handle TIFFOpen errors ...
}

unique_ptr<uint16_t> tiff_tiled_file::tile(size_t tile_idx) const {
//	return read_from_tiff(tile_idx);
	return checker_texture();
}

size_t tiff_tiled_file::tile_size() const {
	return 256;  // FIXME: hardcoded value
}

TIFF * tiff_tiled_file::native() const {
	return _tiff;
}

tiff_tiled_file::~tiff_tiled_file() {
	if (_tiff)
		TIFFClose(_tiff);
}

unique_ptr<uint16_t> tiff_tiled_file::read_from_tiff(size_t tile_idx) {
	assert(_tiff);
	size_t const tile_size = TIFFTileSize(_tiff);
	unique_ptr<uint16_t> tile_data{new uint16_t[tile_size/sizeof(uint16_t)]};
	int ret = TIFFReadEncodedTile(_tiff, tile_idx, tile_data.get(), size_t(-1));
	assert(ret != -1);  // TODO: handle read errors
	return tile_data;
}


unique_ptr<uint16_t> tiff_tiled_file::checker_texture() const {
	assert(tile_size() == 256);
	size_t const square_count = 8,
		square_width = tile_size() / square_count;

	vector<uint16_t> pixels;
	size_t const w = create_checker16_texture(pixels, square_count, square_width);
	assert(w == tile_size());

	unique_ptr<uint16_t> buf{new uint16_t[size(pixels)]};
	copy(begin(pixels), end(pixels), buf.get());
	return buf;
}
