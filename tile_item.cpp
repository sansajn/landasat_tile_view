#include <boost/gil.hpp>
#include <QPainter>
#include "tile_stats.hpp"
#include "tile_item.hpp"
using std::move, std::unique_ptr;
using boost::gil::interleaved_view, boost::gil::gray16_view_t, boost::gil::gray16_pixel_t;

tile_item::tile_item(tiff_tiled_file const & tiff, size_t tile_idx) {
	// get tile data
	unique_ptr<uint16_t> tile_data = tiff.tile(tile_idx);
	size_t const tile_w = tiff.tile_size();  // in pixels

	// create QPixmap
	QImage tile_img(reinterpret_cast<uint8_t *>(tile_data.get()), tile_w, tile_w, sizeof(uint16_t)*tile_w, QImage::Format_Grayscale16);
	_pixels = QPixmap::fromImage(move(tile_img));  // TODO: check whether _pixels are data owner

	if (!_checker_replacement) {  // print stats for tiff tiles
		gray16_view_t pixel_view = interleaved_view(tile_w, tile_w,
			reinterpret_cast<gray16_pixel_t *>(tile_data.get()), sizeof(gray16_pixel_t)*tile_w);
		print_tile_stats(pixel_view);
	}
}

QRectF tile_item::boundingRect() const {
	return QRectF(0, 0, _pixels.width(), _pixels.height());
}

void tile_item::paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) {
	paint->drawPixmap(0, 0, _pixels.width(), _pixels.height(), _pixels);
}
