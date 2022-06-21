#pragma once
#include <memory>
#include <filesystem>
#include <cstdint>
#include <tiffio.h>

//! Landsat8 tiled TIFF file support.
class tiff_tiled_file {
public:
	tiff_tiled_file(std::filesystem::path const & fname);
	std::unique_ptr<uint16_t> tile(size_t tile_idx) const;
	size_t tile_size() const;  //!< \return Tile size in pixels.
	TIFF * native() const;  //!< \note Ideally function should be never used.
	~tiff_tiled_file();

private:
	std::unique_ptr<uint16_t> read_from_tiff(size_t tile_idx);  // TODO: make it const if possible
	std::unique_ptr<uint16_t> checker_texture() const;  //!< for testing purpose, instead of tiff data return checker texture

	TIFF * _tiff;
};
